#include "cardfile.h"

#include <stdlib.h>

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

int NEAR PASCAL LogHimetricToPels(HDC hDC,BOOL bHoriz, int mmHimetric);

extern char szCardfileSect[];
char szIntl[] = "intl";
char    szMeasure[] = "iMeasure";
int     iMeasure = 1;   /* default system of measurement is English, 0 for metric */

int fNeedToUpdateObject = FALSE;
DWORD   CdHt    =   0;
DWORD   CdWd    =   0;
BOOL    fOLE;

/* use this buffer for reading in text from a card or as a scratch buffer */
char szText[512];       

int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpszCommandLine,
    int cmdShow)
{
    MSG msg;
    HDC hDC;
    char szBuf[10];
//    VOID ( *lpfnRegisterPenApp)(WORD, BOOL) = NULL;

    fOLE = GetWinFlags() & WF_PMODE;    /* Are we in real mode today? */
    fOldCardfile = !fOLE;        /* Initialize old/new cardfile flag */
    Hourglass(TRUE);

    hIndexInstance = hInstance;
    /* Make a fixed font. */
    hFont = GetStockObject(SYSTEM_FIXED_FONT);

    if (!IndexInit())
    {
         Hourglass(FALSE);
         goto InitError;
    }

    /* Create a seperate hbrCard and hbrBorder for each instance */
    hbrCard = GetSysColorBrush(COLOR_WINDOW);
    hbrBorder = GetSysColorBrush(COLOR_WINDOWFRAME);

/* Bug 16470:  Register "PenAware" before creating any windows */
//    if (lpfnRegisterPenApp = GetProcAddress(GetSystemMetrics(SM_PENWINDOWS),
//                "RegisterPenApp"))
//        (*lpfnRegisterPenApp)(1, TRUE);

    if (InitInstance(hInstance, lpszCommandLine, cmdShow))
    {
        Hourglass(FALSE);
        while(TRUE)
        {
            if (IsWindow(hEditWnd) && !PeekMessage(&msg, hEditWnd, WM_KEYFIRST, WM_KEYLAST, FALSE))
            {
                if (fNeedToUpdateObject)
                {
                    hDC = GetDC(hEditWnd);
                    SetBkColor(hDC, GetSysColor(COLOR_WINDOW));
                    SetTextColor(hDC, GetSysColor(COLOR_WINDOWTEXT));
                    CardPaint(hDC);
                    ReleaseDC(hEditWnd, hDC);
                    fNeedToUpdateObject = FALSE;
                }
            }
            if (!ProcessMessage(hIndexWnd, hAccel))
                break;
        }
        Hourglass(TRUE);
    }
    else
    {
InitError:
        MessageBox(NULL, NotEnoughMem, NULL, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
    }
//    if (lpfnRegisterPenApp)
//        (*lpfnRegisterPenApp)(1, FALSE);

    /* Free any global memory used by Printer Setup. */
    if(PD.hDevMode)
	GlobalFree(PD.hDevMode);
    if(PD.hDevNames)
	GlobalFree(PD.hDevNames);

    wsprintf(szBuf, "%d",  fValidate);
    WriteProfileString(szCardfileSect, szValidateFileWrite, szBuf);
    
    Hourglass(FALSE);
    return(0);
}

/*
 * Set card count on the right top corner 
 */
void SetNumOfCards(
    void)
{
    char szString[20];
    char szWndText[50];

    if (cCards == 1)
        LoadString(hIndexInstance, IONECARD, szWndText, sizeof(szWndText));
    else
    {
        LoadString(hIndexInstance, ICARDS, szString, sizeof(szString));
        wsprintf(szWndText, "%d %s", cCards, (LPSTR)szString);
    }
    SetWindowText(hRightWnd, szWndText);
}

/* Why is it here?  Because the DWORD mul/div routines are in _TEXT */
void FixBounds(
    LPRECT lprc) 
{
    HDC hDC;
    DWORD xDiff;
    DWORD yDiff;

    if (!CdHt) 
    {
        CdHt = (CARDLINES * CharFixHeight);
        CdWd = (LINELENGTH * CharFixWidth);
    }


    /* First map from HIMETRIC back to screen coordinates */
    hDC = GetDC(NULL);

    lprc->right  = LogHimetricToPels(hDC, TRUE,  lprc->right);
    lprc->bottom = LogHimetricToPels(hDC, FALSE, -lprc->bottom);

    ReleaseDC(NULL, hDC);

    /* Preserve the Aspect Ratio of the picture */
    xDiff = (DWORD) (lprc->right - lprc->left + 1);
    yDiff = (DWORD) (lprc->bottom - lprc->top + 1);

    /* Don't use *= here because of integer arithmetic... */
    if ((xDiff > CdWd) || (yDiff > CdHt)) 
    {
        if ((xDiff * CdHt) > (yDiff * CdWd)) 
        {
            yDiff = ((yDiff * CdWd) / xDiff);
            xDiff = CdWd;
        } 
        else 
        {
            xDiff = ((xDiff * CdHt) / yDiff);
            yDiff = CdHt;
        }
    }
    SetRect(lprc, 0, 0, (int)xDiff - 1, (int)yDiff - 1);
}

/* ProcessMessage() - Spin in a message dispatch loop.
 */
BOOL ProcessMessage(
    HWND hwndFrame, 
    HANDLE hAccTable) 
{
    BOOL    fReturn;
    MSG     msg;

    if (fReturn = GetMessage((LPMSG)&msg, NULL, 0, 0)) 
    {
        if (!hDlgFind || !IsDialogMessage(hDlgFind, &msg))
        {
            if (!TranslateAccelerator(hIndexWnd, hAccel, &msg)) 
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    return fReturn;
}

int Scale(
    int coord, 
    int s1, 
    int s2) 
{
    return ((int) (((LONG)coord * (LONG)s1) / (LONG)s2));
}

BOOL IndexOkError(
    int strid)
{
    char buf[400];
    LPSTR lpbuf;

    if (strid == EINSMEMORY)
        lpbuf = (LPSTR)NotEnoughMem;
    else if(!LoadString(hIndexInstance, strid, buf, sizeof(buf)))
    	lpbuf = (LPSTR)NotEnoughMem;
    else if (strid == E_FLOPPY_WITH_SOURCE_REMOVED)
      {
        WORD wLen = lstrlen((LPSTR)buf);

        if(!LoadString(hIndexInstance, strid+1, buf + wLen, sizeof(buf) - wLen))
    	    lpbuf = (LPSTR)NotEnoughMem;
        else
    	    lpbuf = (LPSTR)buf;
      }
    else
    	lpbuf = (LPSTR)buf;
    MessageBox(hIndexWnd, lpbuf, szWarning, MB_OK | MB_ICONEXCLAMATION);
    return FALSE; /* caller can return this to indicate failure */
}

/* Scan lpSrc for merge spec.    
 * If found, insert string lpMerge at that point.
 * Then append rest of lpSrc.
 * NOTE! Merge spec guaranteed to be two chars.
 *     returns TRUE if it does a merge, false otherwise. 
 */
BOOL MergeStrings(
    LPSTR lpSrc, 
    LPSTR lpMerge, 
    LPSTR lpDst)
{
    LPSTR lpMatch;
    int nChars;

    /* Find merge spec */
    if (!(lpMatch = StrChr(lpSrc, wMerge)))
        return FALSE;       /* none found */

    /* copy from src to dest, upto the merge spec char */
    nChars = lpMatch - lpSrc;
    StrCpyN(lpDst, lpSrc, nChars);
    lpDst += nChars;

    /* If merge spec found, insert string to be merged. */
    if (lpMerge)
    {
        lstrcpy(lpDst, lpMerge);
        lpDst += lstrlen(lpMerge);
    }

    /* copy rest of the source after the merge spec */
    lstrcpy(lpDst, lpMatch+2);
    return TRUE;
}

void MakeBlankCard(
    void)
{
    CurCardHead.line[0] = 0;
    SavedIndexLine[0] = 0;
    szText[0] = 0;
    CurCard.lpObject = NULL;
    SetEditText("");
    CurCardHead.flags = FNEW;
}

void SetCaption(
    void)
{
    char buf[190];

    BuildCaption(buf);
    SetWindowText(hIndexWnd, buf);
}

/*
 * build a string to display in the caption bar
 *
 * note: the caption is used in the printing options.  check there
 * before you change this.
 *
 */
void BuildCaption(
    char *pBuf)
{
    LPSTR pFile;

    pFile = (*CurIFile) ? FileFromPath(CurIFile) : (LPSTR)szUntitled;
    wsprintf(pBuf, "%s - %s", (LPSTR)szCardfile, (LPSTR)pFile);
}

/* max size of short date format from control panel - dd/MM/yyyy */
typedef struct tagDATEFORMAT
{
    char *szFormats[3];   /* short date format strings for MDY, DMY, YMD */
    int Flags;            /* flag indicating century,lead zeros for month and day. */
} DATEFORMAT;

/* Get country info from win.ini file, and initialize global vars
      that determine format of date/time string. */
void FAR InitCountry(
    void)
{
#define MAX_DATE_FORMATS        8
    static DATEFORMAT DateFormats[MAX_DATE_FORMATS] =
    {{{"M/d/yy",    "d/M/yy",    "yy/M/d"    }, 0},
     {{"M/dd/yy",   "dd/M/yy",   "yy/M/dd"   }, LEADZERODAY},
     {{"MM/d/yy",   "d/MM/yy",   "yy/MM/d"   }, LEADZEROMONTH},
     {{"MM/dd/yy",  "dd/MM/yy",  "yy/MM/dd"  }, LEADZERODAY | LEADZEROMONTH},
     {{"M/d/yyyy",  "d/M/yyyy",  "yyyy/M/d"  }, CENTURY},
     {{"M/dd/yyyy", "dd/M/yyyy", "yyyy/M/dd" }, LEADZERODAY | CENTURY},
     {{"MM/d/yyyy", "d/MM/yyyy", "yyyy/MM/d" }, LEADZEROMONTH | CENTURY},
     {{"MM/dd/yyyy","dd/MM/yyyy","yyyy/MM/dd"}, LEADZERODAY | LEADZEROMONTH | CENTURY}
    };
    int  i, id;
    char szT[2];
    char szDateFormat[MAX_FORMAT];
    char *p;

    extern TIME Time;
    extern CARDDATE Date;
    
    /* date format, 0 => MDY, 1=> DMY, 2=> YMD */
    Date.Order=GetProfileInt(szIntl, "iDate", 0);

    /* get char for date seperator */
    GetProfileString(szIntl, "sDate", "/", szT,2);
    Date.chSep = *szT;

    /* short date format, determines LeadZeroDay, LeadZeroMonth, Century */
    GetProfileString(szIntl, "sShortDate", "d/M/yy", Date.szShortFormat, 11);
    /* make private copy and replace current date seperator with our own('/')*/
    lstrcpy(szDateFormat, Date.szShortFormat);
    if (Date.chSep != '/')
    {
        p = szDateFormat;
        while (*p)
        {
            if (*p != 'M' && *p != 'd' && *p != 'y')
                *p = '/';
            p++;
        }
    }

    Date.Flags = 0;    /* assume "d/M/yy" */
    /* match szDateFormat and pick up the corresponding flags */
    for (i = 0; i < MAX_DATE_FORMATS; i++)
    {
        if (lstrcmpi(DateFormats[i].szFormats[Date.Order], szDateFormat) == 0)
        {
            Date.Flags = DateFormats[i].Flags;    
            break;
        }
    }

    /* string for AM */
    GetProfileString(szIntl, "s1159", "AM", Time.sz1159, 6);
    /* string for PM */
    GetProfileString(szIntl, "s2359", "PM", Time.sz2359, 6);
    /* set flag for 24 hr time format */
    Time.f24Hr = GetProfileInt(szIntl, "iTime", 0);
    /* time seperator */
    GetProfileString(szIntl, "sTime", ":", szT,2);
    Time.chSep = *szT;
    /* leading zero for hour */
    Time.fLeadZeroHour = GetProfileInt(szIntl, "iTLzero", 0);

    GetProfileString(szIntl, "sDecimal", ".", szDec, 2);
    /* Scan for . and replace with intl decimal */
    for (id=2; id<6; id++)
    {
        for (i=0; i < lstrlen(chPageText[id]); i++)
            if (chPageText[id][i]=='.')
                chPageText[id][i]=szDec[0];
    }
}

/*
 * check for printer availablilty
 */
void IndexWinIniChange(
    void)
{
    HANDLE hMenu;
    short id, nx;
    char  ch;
    char msgbuf[120];
    int fEnabled = MF_GRAYED;

    static bszDecRead=FALSE;

    /* Get system of measurement, metric/English(0/1) */
    iMeasure = GetProfileInt(szIntl, szMeasure, 1);

    /* Set decimal to scan for */
    if (bszDecRead)
        ch=szDec[0]; /* If we already changed it. */
    else
        ch='.';  /* First time. */

    bszDecRead=TRUE;

    /* Get the intl decimal character for use in Page Setup Box. */
    GetProfileString(szIntl, "sDecimal", ".", szDec, 4);
    /* Scan for . and replace with intl decimal */
    for (id=2; id<6; id++)
    {
        for (nx=0; nx < lstrlen(chPageText[id]); nx++)
            if (chPageText[id][nx]==ch)
                chPageText[id][nx]=szDec[0];
    }

    hMenu = GetMenu(hIndexWnd);

/* Bug 8017:  If we're setup for the default printer, throw away whatever
 * we've got as the settings may have changed.  We'll grab the new default
 * immediately before we print.     Clark Cyr    5 December 1991
 */
    if (PD.hDevNames)
    {
        BOOL bIsDefault;
        LPDEVNAMES lpDevNames  = (LPDEVNAMES)GlobalLock(PD.hDevNames);

        bIsDefault = lpDevNames->wDefault & DN_DEFAULTPRN;
        GlobalUnlock(PD.hDevNames);
        if (bIsDefault)
            FreePrintHandles();  /* bPrinterSetupDone is set to NULL */
    }

    if (bPrinterSetupDone ||
        GetProfileString(szWindows, szDevice, DefaultNullStr, msgbuf, 120))
    {
        fCanPrint = TRUE;
    }
    else
        fCanPrint = FALSE;

    InitCountry();
}

BOOL BuildAndDisplayMsg( int idError, 
                         char szString[])
{
    char szError[300];
    char szMsg[460];

    /* load specified error msg */
    LoadString(hIndexInstance, idError, szError, sizeof(szError));
    /* Merge in the given string into the error msg */
    MergeStrings(szError, szString, szMsg);
    MessageBox(hIndexWnd, szMsg, szNote, MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
}

long Myatol(
    char *szNum)
{
    return atol(szNum);
}

OLESTATUS OleStatusCallBack = S_OK;

int CallBack(
    LPOLECLIENT lpclient,
    OLE_NOTIFICATION flags,
    LPOLEOBJECT lpObject)
{
    OLE_RELEASE_METHOD method;
    WORD fOleErrMsg;

    switch(flags) 
    {
        case OLE_CLOSED:
            /* After an InsertObject, the server was closed without updating the
             * embedded object */
            if (fInsertComplete == FALSE)
                PicDelete(&CurCard);

            fInsertComplete = TRUE;
            break;

        case OLE_SAVED:
        case OLE_CHANGED:
            /* in case we did an InsertObject, the object is updated now. */
            fInsertComplete = TRUE;
            /*
            * The OLE libraries make sure that we only receive
            * update messages according to the Auto/Manual flags.
            */
            CurCardHead.flags |= FDIRTY;
            InvalidateRect(hEditWnd, NULL, TRUE);
            CurCard.rcObject.right = 0;        /* Force resize calculation */
            break;

        case OLE_RELEASE:
            --cOleWait;

            method = OleQueryReleaseMethod(lpObject);
            if (method == OLE_LOADFROMSTREAM)
            {
                oleloadstat = OleQueryReleaseError(lpObject);
                break;
            }
            else if (method == OLE_SETDATA ||
                    method == OLE_UPDATE) 
            {
                OleStatusCallBack = OleQueryReleaseError(lpObject);
                break;
            }

            switch (fOleErrMsg = OleError(OleQueryReleaseError(lpObject))) 
            {
                case FOLEERROR_NOTGIVEN:
                case FOLEERROR_GIVEN:
                    switch (OleQueryReleaseMethod(lpObject))
                    {
                        case OLE_CREATEFROMFILE:
                            PicDelete(&CurCard);
                            InvalidateRect(hEditWnd, NULL, TRUE);
                            if (fOleErrMsg == FOLEERROR_NOTGIVEN)
                                ErrorMessage(E_DRAG_DROP_FAILED);
                            break;

                        default:
                            break;
                    }
                    break;

                case FOLEERROR_OK:
                    switch (OleQueryReleaseMethod(lpObject)) 
                    {
                        case OLE_SETUPDATEOPTIONS:
                            if (hwndLinkWait)
                                PostMessage(hwndLinkWait, WM_COMMAND, IDD_LINKDONE, 0L);
                            default:
                                break;
                    }

                default:
                    break;
           }
           break;

        case OLE_QUERY_RETRY:
        case OLE_QUERY_PAINT:
            return TRUE;    /* Always continue painting, for now */
            break;

        default:
            break;
    }
    return 0;
}

int LogHimetricToPels(HDC hDC,BOOL bHoriz, int mmHimetric)
{
    #define nHMPerInch 2540
    #define nPelsPerLogInch GetDeviceCaps(hDC, bHoriz ? LOGPIXELSX : LOGPIXELSY)

    return MulDiv(mmHimetric,nPelsPerLogInch,nHMPerInch);
}
