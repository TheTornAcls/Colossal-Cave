#include "cardfile.h"
#include "shellapi.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/* OLE - Paste special support                                          */
/************************************************************************/

NOEXPORT BOOL GetClipObjectInfo( LPSTR szClass, 
    LPSTR szItem);

NOEXPORT BOOL SetupFormatsList( HWND hwndList);

#define MAX_CLIPFORMATS     20
int Formats[MAX_CLIPFORMATS];
int nFormats, iFormats;
BOOL fPaste;
int ClipFormat;

void DoPasteSpecial( void )
{
    DLGPROC lpfn;
    int Result;

    if (EditMode == I_TEXT) 
    {
        return;
    }

    if (!(lpfn = MakeProcInstance(PasteSpecialDlgProc, hIndexInstance)))
    {
        return;
    }

    Result = DialogBox(hIndexInstance, MAKEINTRESOURCE(DLGPASTESPECIAL), hIndexWnd, lpfn);
    FreeProcInstance(lpfn);
    if (Result == -1)
    {
        return; /* probably error msg , out of mem */
    }
    else if (Result == 0)
    {
        return;   /* dlg cancelled */
    }

    if (ClipFormat == vcfOwnerLink)
    {
        PicPaste(&CurCard, fPaste, NULL );  
    }
    else
    {
        PicPaste(&CurCard, fPaste, ClipFormat );  
    }
    //PicPaste(&CurCard, fPaste, ((ClipFormat == vcfOwnerLink) ?
    //                                NULL : ClipFormat));  
}

INT_PTR CALLBACK PasteSpecialDlgProc( HWND hDlg, 
                                      UINT message, 
                                      WPARAM wParam, 
                                      LPARAM lParam)
{
    int i;
    static BOOL fLinkAvail, fEmbAvail, fObjAvail;

    switch (message)
    {
        case WM_INITDIALOG:
        {
            char szClassName[KEYNAMESIZE];
            char szItemName[PATHMAX];
            HWND hwndList;
            int nSelect;
            
            hwndList = GetDlgItem(hDlg,IDD_LISTBOX);
            szClassName[0] = szItemName[0] = '\0';
            iFormats = 0; /* always start with 0 */
            ClipFormat = NULL;  /* no formats have been selected */
            
            fEmbAvail  = OleQueryCreateFromClip(szPStdFile, olerender_draw, 0) == S_OK;
            fLinkAvail = OleQueryLinkFromClip(szPStdFile, olerender_draw, 0) == S_OK;
            fObjAvail = fEmbAvail || fLinkAvail;

            if (fObjAvail)
            {
                char szTmp[OBJSTRINGSMAX];
                char szListItem[OBJSTRINGSMAX+KEYNAMESIZE]; // hope this is big enough!

		        GetClipObjectInfo(szClassName, szItemName);
		        SetWindowText(GetDlgItem(hDlg,IDD_CLIPOWNER), szClassName);
		        SetWindowText(GetDlgItem(hDlg,IDD_ITEM), szItemName);
	            LoadString(hIndexInstance, IDS_OBJECTFMT, szTmp, sizeof(szTmp));
                wsprintf(szListItem,"%s %s",(LPSTR)szClassName,(LPSTR)szTmp);
                SendMessage(hwndList, LB_INSERTSTRING, iFormats, (DWORD)(LPSTR)szListItem);
                Formats[iFormats++] = vcfOwnerLink;
            }
            else
            {
		        ShowWindow(GetDlgItem(hDlg,IDD_SOURCE), SW_HIDE);
            }

            SetupFormatsList(hwndList);

            /* if Ownerlink/ObjectLink available choose that as the default */
            if (fObjAvail)
            {
                nSelect = 0;
            }
            else    /* no Ole Object, choose a static object */
            {
                nSelect = 1;
            }
            SendMessage(hwndList, LB_SETCURSEL, nSelect, 0L);
            EnableWindow(GetDlgItem(hDlg, IDD_PASTELINK), fLinkAvail);

            return TRUE;
        }
        case WM_COMMAND:
            switch (wParam)
            {
                case IDD_LISTBOX:
                    switch (HIWORD(lParam))
                    {
                        case LBN_DBLCLK:
                            SendMessage(hDlg,WM_COMMAND,IDD_PASTE,0L);
                            return TRUE;

                        default:
                            break;
                    }
                    break;

                case IDD_PASTE:
                case IDD_PASTELINK:
                    i = (WORD)SendDlgItemMessage(hDlg, IDD_LISTBOX, LB_GETCURSEL, 0, 0L);
                    if (i == LB_ERR)
                    {
                        break;
                    }

                    fPaste = (wParam == IDD_PASTE);
                    ClipFormat = Formats[i];
                    EndDialog(hDlg, TRUE);
                    break;

                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    break;
            }
            break;
    }
    return FALSE;
}

/* fill hwndList with all the formats available on clipboard */
NOEXPORT BOOL SetupFormatsList( HWND hwndList)
{ 
	CHAR szPicture[OBJSTRINGSMAX];
	CHAR szBitmap[OBJSTRINGSMAX];

    WORD cfFormat = NULL;

    if (!OpenClipboard(hIndexWnd))
    {
        return FALSE;
    }

	LoadString(hIndexInstance, IDS_PICTUREFMT, szPicture, OBJSTRINGSMAX);
	LoadString(hIndexInstance, IDS_BITMAPFMT, szBitmap, OBJSTRINGSMAX);

    while (cfFormat = EnumClipboardFormats(cfFormat))
    {
        switch(cfFormat)
        {
            case CF_BITMAP:
                SendMessage(hwndList, LB_INSERTSTRING, iFormats, (DWORD)(LPSTR)szBitmap);
                Formats[iFormats++] = cfFormat;
                break;

            case CF_METAFILEPICT:
                SendMessage(hwndList, LB_INSERTSTRING, iFormats, (DWORD)(LPSTR)szPicture);
                Formats[iFormats++] = cfFormat;
                break;

            default:
                break; //do nothing
        } //end switch
    }

    CloseClipboard();
    return iFormats;
}

/* get the classname, item name for the owner of the clipboard */
/* return TRUE if successful */
/* only gets ownerlink class, assumes its available */
NOEXPORT BOOL GetClipObjectInfo(LPSTR szClass, LPSTR szItem)
{
	HANDLE hData=NULL;
    LPSTR lpData=NULL;
    char szFullItem[PATHMAX],*pch;
    LONG dwSize = KEYNAMESIZE;

	if (!OpenClipboard(hIndexWnd))
        return FALSE;

    if (!(hData = GetClipboardData(vcfOwnerLink)) &&
        !(hData = GetClipboardData(vcfLink)))
    {
        CloseClipboard();
        return FALSE;
    }


    if (!(lpData = (LPSTR)GlobalLock(hData)))
    {
        CloseClipboard();
        return FALSE;
    }

    /**** get szClass ****/
    if (RegQueryValue(HKEY_CLASSES_ROOT, lpData, szClass, &dwSize))
	    lstrcpy(szClass, lpData);

    /**** get szName ****/
    while(*lpData++); // skip class key

    pch = szFullItem;

    /* first doc name */
    do
    {
       *pch++ = *lpData; 
    } while(*lpData++);

    /* second item name (if there) */
    if (*lpData)
    {
        *(pch-1) = ' ';
        do
        {
            *pch++ = *lpData; 
        } while(*lpData++);
    }

    /* get rid of path.  pch now points to \0 */
#ifdef DBCS
    // in DBCS world, we should use AnsiPrev(...)
    // instead of --pch. LiZ -- 5/14/93
    pch = AnsiPrev( szFullItem, pch );
    while (pch != szFullItem)
    {
        if ((*pch == '\\') || (*pch == ':'))
        {
            pch++;
            break;
        }
        else
            pch = AnsiPrev( szFullItem, pch );
    }
    if ((*(pch) == '\\') || (*(pch) == ':'))	//02/26/93 T-HIROYN
		pch++;
#else
    --pch;
    while (pch != szFullItem)
    {
        if ((*(pch-1) == '\\') || (*(pch-1) == ':'))
            break;
        else
            --pch;
    }
#endif

    lstrcpy(szItem,(LPSTR)pch);

    GlobalUnlock(hData);
	CloseClipboard();

    return TRUE;
}