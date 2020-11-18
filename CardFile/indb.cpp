/* Dialog Procedures */

#include "cardfile.h"
#include "dlgs.h"               /* Common dialog ids */

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

int DBcmd;
BOOL fRecursive;        /* Avoid recursive IDD_BUTTON calls */
BOOL fValidDB;          /* Are we validating on save, or not?  local to DB */
HWND hwndLinkWait;      /* Waits for "Set Link" callback */
BOOL fAutomatic;        /* Did the link get changed to Automatic? */

BOOL DlgProc(
            HWND hDB,
            unsigned message,
            WORD wParam,
            LONG lParam)
{
    char *pResultBuf;
    char *pInit;
    int len;

    switch (message)
    {
    case WM_INITDIALOG:
        switch (DBcmd)
        {
        case DTHEADER:
            pInit = CurCardHead.line;
            break;

        default:
            pInit = "";
        }
        SendDlgItemMessage(hDB, ID_EDIT, EM_LIMITTEXT, LINELENGTH, 0L);
        SetDlgItemText(hDB, ID_EDIT, pInit);

        SetFocus(GetDlgItem(hDB, ID_EDIT));
        return(TRUE);

    case WM_COMMAND:
        pResultBuf = NULL;
        switch (wParam)
        {
        case IDOK:
            if ((len = GetWindowTextLength(GetDlgItem(hDB, ID_EDIT))) || DBcmd == DTHEADER || DBcmd == DTADD)
                if (pResultBuf = (char *)LocalAlloc(LPTR, ++len))
                    GetDlgItemText(hDB, ID_EDIT, pResultBuf, len);
            break;
        case IDCANCEL:
            break;
        default:
            return(FALSE);
        }       
        EndDialog(hDB, (int)pResultBuf);     /* return pointer to buffer */
        return(TRUE);
        break;
    default:
        return(FALSE);
    }
}

/* LinksDlg... dialog */
BOOL fnLinksDlg(
               HWND hDlg,
               unsigned message,
               WORD wParam,
               LONG lParam)
{
    DWORD fUpdate;
    HANDLE hData = NULL;
    LPSTR lpstrData = NULL;

    switch (message)
    {
    case WM_INITDIALOG:         /* Clone the object */
        /*
         * Two ways to implement undo:  Either save when the dialog pops up,
         * or save when an action is taken.  I choose to save when the
         * dialog pops up.
         */
        hwndError = hDlg;
        fAutomatic = FALSE;
        PicSaveUndo(&CurCard);

        //TODO: Use IOleLink::GetLinkOptions

    case WM_REDRAWDIALOG:
        if (S_OK != OleGetLinkUpdateOptions(CurCard.lpObject, &fUpdate))
            ErrorMessage(W_IMPROPER_LINK_OPTIONS);
        else
        {
            if (fUpdate == oleupdate_always)
                CheckRadioButton(hDlg, IDD_AUTO, IDD_MANUAL, IDD_AUTO);
            else if (fUpdate == oleupdate_oncall)
                CheckRadioButton(hDlg, IDD_AUTO, IDD_MANUAL, IDD_MANUAL);
            else
                ErrorMessage(W_IMPROPER_LINK_OPTIONS);
        }

        // Is this OleGetClipBoard
        if (S_OK != OleGetData(CurCard.lpObject, vcfLink, &hData))
        {
            ErrorMessage(E_GET_FROM_CLIPBOARD_FAILED);
            return TRUE;
        }

        /* link formats :  "szClass0szDocument0szItem00" */
        if (lpstrData = (LPSTR) GlobalLock(hData))
        {
            LPSTR lpstrTemp;

            GetClassId(GetDlgItem(hDlg, IDD_CLASSID), lpstrData);
            /* display the Document and Item names */

            while (*lpstrData++)
                continue;
            /* Strip off the path name and drive letter */
            lpstrTemp = lpstrData;
            while (*lpstrTemp)
            {
                if (*lpstrTemp == '\\' || *lpstrTemp == ':')
                    lpstrData = lpstrTemp + 1;
                lpstrTemp = AnsiNext(lpstrTemp);
            }

            SetWindowText(GetDlgItem(hDlg, IDD_DOCINFO), lpstrData);
            while (*lpstrData++)
                continue;

            SetWindowText(GetDlgItem(hDlg, IDD_ITEMINFO), lpstrData);
            GlobalUnlock(hData);
        }
        return TRUE;

    case WM_COMMAND:
        switch (wParam)
        {
        case IDD_FREEZE:        /* Cancel Link */
        case IDD_UPDATE:        /* Update the Link */

            /* Only update menus if successful */
            if (SendMessage(hIndexWnd, WM_COMMAND, wParam, 0L)
                && wParam == IDD_FREEZE)
            {
                EnableWindow(GetDlgItem(hDlg, IDD_UPDATE), FALSE);
                EnableWindow(GetDlgItem(hDlg, IDD_CHANGE), FALSE);
                CheckDlgButton(hDlg, IDD_AUTO, FALSE);
                CheckDlgButton(hDlg, IDD_MANUAL, FALSE);
                EnableWindow(GetDlgItem(hDlg, IDD_AUTO), FALSE);
                EnableWindow(GetDlgItem(hDlg, IDD_MANUAL), FALSE);
            }

            EnableWindow(GetDlgItem(hDlg, IDCANCEL), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDD_EDIT), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDD_PLAY), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDD_FREEZE), FALSE);
            break;

        case IDD_CHANGE:
            if (GetNewLinkName(hDlg, &CurCard))
                PostMessage(hDlg, WM_REDRAWDIALOG, wParam, 0L);
            else
            {
                SendMessage(hIndexWnd, WM_COMMAND, UNDO, 0L);
                PicSaveUndo(&CurCard);
            }
            break;

        case IDD_PLAY:
        case IDD_EDIT:
            PostMessage(hIndexWnd, WM_COMMAND, wParam, 0L);
            goto Dismiss;

        case IDD_AUTO:
        case IDD_MANUAL:
            if ((IsDlgButtonChecked(hDlg, IDD_AUTO) && wParam == IDD_AUTO)
                || (IsDlgButtonChecked(hDlg, IDD_MANUAL) && wParam == IDD_MANUAL)
                || fRecursive)
                break;

            fRecursive = TRUE;

            switch (OleSetLinkUpdateOptions(CurCard.lpObject, 
                                            wParam == IDD_AUTO ? oleupdate_always : oleupdate_oncall))
            {
            case OLE_WAIT_FOR_RELEASE:
                hwndLinkWait = hDlg;        
                break;

            case S_OK:
                fRecursive = FALSE;
                goto LinkDone;
                break;

            case OLE_BUSY:
                ErrorMessage(E_SERVER_BUSY);
                /* Fall through */

            default:
                ErrorMessage(E_IMPROPER_LINK_OPTIONS);
                break;
            }
            fRecursive = FALSE;
            break;

        case IDD_LINKDONE:
            LinkDone:       
            if (IsDlgButtonChecked(hDlg, IDD_MANUAL))
            {
                fAutomatic = TRUE;
                CheckRadioButton(hDlg, IDD_AUTO, IDD_MANUAL, IDD_AUTO);
            }
            else
            {
                CheckRadioButton(hDlg, IDD_AUTO, IDD_MANUAL, IDD_MANUAL);
            }
            hwndLinkWait = NULL;
            return FALSE;

        case IDCANCEL:
            PostMessage(hIndexWnd, WM_COMMAND, UNDO, 0L);
            goto Dismiss;

        case IDOK:
            /* fAutomatic is set only if the button changed to Automatic */
            if (fAutomatic && IsDlgButtonChecked(hDlg, IDD_AUTO))
            {
                PostMessage(hIndexWnd, WM_COMMAND, IDD_UPDATE, 0L);
            }
            OleError(OleClose(lpObjectUndo));

            Dismiss:        
            hwndError = hIndexWnd;
            EndDialog(hDlg, TRUE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

/* Invalid Link dialog */
int fnInvalidLink(
                 HWND hDlg,
                 unsigned message,
                 WORD wParam,
                 LONG lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetWindowText(hDlg, szWarning);
        return TRUE;
        break;

    case WM_COMMAND:
        switch (wParam)
        {
        case IDOK:
        case IDD_LINK:
            {
                EndDialog(hDlg, wParam);
            }
        }
    }
    return FALSE;
}

BOOL HookProc(HWND hDlg, unsigned msg, WORD wParam, LONG lParam) 
{
    switch (msg)
    {
    case WM_INITDIALOG: 
    {
            char    szValidate[160];

            LoadString(hIndexInstance, IDS_VALIDATE, szValidate, 160);
            SetDlgItemText(hDlg, chx1, szValidate);
            CheckDlgButton(hDlg, chx1, fValidDB);
            return TRUE;
        }

    case WM_COMMAND:
        if (wParam == chx1)
        {
            fValidDB = !fValidDB;
            return TRUE;
        }
        break;

    default:
        break;
    }
    return FALSE;
}
