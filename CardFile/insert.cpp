#include "cardfile.h"
#include "shellapi.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/* OLE - Insert New Object support                                      */
/************************************************************************/

NOEXPORT int NEAR InitClassNamesList(
    HWND hwndList);

NOEXPORT BOOL NEAR CreateObjectInDoc(
    PCARD pCard,
    char szClass[]);

NOEXPORT BOOL NEAR ServerNameFromClass(
    char szClass[], 
    char szServer[]);

NOEXPORT BOOL NEAR GetSelectedClassName(
    HWND hwndList, 
    char szSelectedClass[]);
/* set to FALSE at the beginning of Insertobject, 
 * set to TRUE in Ole CallBack function on OLE_SAVE/OLE_CHANGE, 
 * If FALSE when OLE_CLOSE received, then delete the Object(server closed without
 * updating the embedded object).
 */
BOOL fInsertComplete=TRUE;

char szClassName[KEYNAMESIZE];
void InsertObject( void)
{
    DLGPROC lpfn;
    int Result;

    if (EditMode == I_TEXT) 
        return;

    if (!(lpfn = MakeProcInstance(InsertObjectDlgProc, hIndexInstance)))
        return;

    /* this'll set global szClassName */
    Result = DialogBox(hIndexInstance, MAKEINTRESOURCE(DLGINSERTOBJECT), 
                hIndexWnd, lpfn);
    FreeProcInstance(lpfn);
    if (-1 == Result )
    {
        return; /* probably error msg , out of mem */
    }
    else if (Result == 0 || szClassName[0] == 0)
    {
        return;   /* dlg cancelled */
    }

    PicSaveUndo(&CurCard);
    CreateObjectInDoc(&CurCard, szClassName);
}

/* Insert New... dialog */
int CALLBACK InsertObjectDlgProc(
    HWND hDlg, 
    UINT msg, 
    WPARAM wParam, 
    LPARAM lParam) 
{
    HWND hwndList;

    switch (msg) 
    {
        case WM_INITDIALOG:
            *szClassName = 0;
            hwndList = GetDlgItem(hDlg, IDD_LISTBOX);
            if (!InitClassNamesList(hwndList))
                EndDialog(hDlg, FALSE);
            SendMessage(hwndList, LB_SETCURSEL, 0, 0L);
            SetFocus(hwndList);
            return FALSE;
            break;

        case WM_COMMAND:
            switch (wParam) 
            {
                case IDD_LISTBOX:
                    if (HIWORD(lParam) != LBN_DBLCLK)
                        break;
                    /* fall through */
                case IDOK:
                    hwndList = GetDlgItem(hDlg, IDD_LISTBOX);
                    if (!GetSelectedClassName(hwndList, szClassName))
                    {
                        EndDialog(hDlg, FALSE); /* error, similar to Cancel dlg */
                    }
                    else
                    {
                        EndDialog(hDlg, TRUE);
                    }

                    break;

                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    break;
            }
            break;
    }
    return FALSE;
}

BOOL CreateObjectInDoc( PCARD pCard,
    char szClass[])
{
    LPOLEOBJECT lpObject;
    OBJECTTYPE otObject;
    bool fOleErrRet;

    wsprintf(szObjectName, szObjFormat, idObjectMax + 1);
    fInsertComplete = FALSE;
    if (fOleErrRet = OleError(OleCreate(szPStdFile, lpclient, (LPSTR)szClass,
                    lhcdoc, szObjectName, &lpObject, olerender_draw, 0)))
    {
        if (fOleErrRet == FOLEERROR_NOTGIVEN)
        {
            ErrorMessage(E_CREATEOBJECTFAILED);
        }

        return FALSE;
    }
    else
        otObject = EMBEDDED;

    DoSetHostNames(lpObject, otObject);

    if (lpObject) 
    {
        if (pCard->lpObject)
            PicDelete(pCard);
        pCard->lpObject = lpObject;
        pCard->otObject = otObject;
        pCard->idObject = idObjectMax++;
        CurCardHead.flags |= FDIRTY;
    }   

    return TRUE;
}

/* InitClassNamesList() - Fills the list box with possible server names. */
int InitClassNamesList(
    HWND hwndList) 
{
    char    szClass[KEYNAMESIZE];
    char    szServer[KEYNAMESIZE];
    HKEY    hkeyRoot;
    int     i;

    if (RegOpenKey(HKEY_CLASSES_ROOT, NULL, &hkeyRoot))
    {
        return FALSE;
    }

    SendMessage(hwndList, LB_RESETCONTENT, 0, 0L);
    for (i = 0; !RegEnumKey(HKEY_CLASSES_ROOT, i, szClass, KEYNAMESIZE); i++)
    {
        if (ServerNameFromClass(szClass, szServer))
            SendMessage(hwndList, LB_ADDSTRING, 0, (DWORD)(LPSTR)szServer);
    }

    RegCloseKey(hkeyRoot);
    return SendMessage(hwndList, LB_GETCOUNT, 0, 0L);
}

/* for a given classname, it returns the server name if one exists.
 * returns TRUE on success, FALSE if given classname is not a server */
BOOL ServerNameFromClass(
    char szClass[], 
    char szServer[])  
{
    LONG   dwSize = KEYNAMESIZE;
    HKEY    hkeyTemp = NULL;
    char    szExec[KEYNAMESIZE];
    int     Result;

    if (*szClass != '.')           /* Not default extension... */
    {
        /* See if this class really refers to a server */
        wsprintf(szExec, "%s\\protocol\\StdFileEditing\\server", (LPSTR)szClass);
        if (!RegOpenKey(HKEY_CLASSES_ROOT, szExec, &hkeyTemp)) 
        {
            /* get the class name string */
            Result = RegQueryValue(HKEY_CLASSES_ROOT, szClass, szServer, &dwSize); 
            RegCloseKey(hkeyTemp);
            if (Result == 0)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/* GetSelectedClassName() - Returns the ASCII class id from the listbox.
 */
BOOL GetSelectedClassName(
    HWND hwndList, 
    char szSelectedClass[]) 
{
    char    szKey[KEYNAMESIZE];
    char    szClass[KEYNAMESIZE];
    char    szServer[KEYNAMESIZE];
    int     i;
    int     Sel;
    HKEY    hkeyRoot;

    if (RegOpenKey(HKEY_CLASSES_ROOT, NULL, &hkeyRoot))
        return FALSE;

    Sel = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0L);
    SendMessage(hwndList, LB_GETTEXT, Sel, (DWORD)(LPSTR)szKey);

    for (i = 0; 
        !RegEnumKey(HKEY_CLASSES_ROOT, i, szClass, KEYNAMESIZE); i++)
    {
        if (ServerNameFromClass(szClass, szServer) &&   /* if it is a server */
            lstrcmp(szServer, szKey) == 0)              /* and it is the one we want */
        {
            lstrcpy(szSelectedClass, szClass);
            return TRUE;
        }
    }

    RegCloseKey(hkeyRoot);
    return FALSE;
}

