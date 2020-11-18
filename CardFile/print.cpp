#include "cardfile.h"

#include <drivinit.h>
#include <cderr.h>

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985 - 2003 - All Rights Reserved     */
/*                                                                      */
/************************************************************************/

NOEXPORT void NEAR PrintHeaderFooter(
    HDC hDC, 
    int i);

NOEXPORT void GetDateTime(char *szTime, char *szDate);

#define MAXPROFILELEN   60

char DefaultNullStr[] = "" ; 

HWND hAbortDlgWnd;
int fAbort;
int bError;
int iTabSize;

int xPrintChar;         /* width of char on printer */
int yPrintChar;         /* height of char on printer */
int xPrintCard;         /* width of card on printer */
int yPrintCard;         /* height of card on printer */

int ExtPrintLeading;
int xHeadFoot;
BOOL bCenter, bRight;
int xPrintRes;
int yPrintRes;
int yPixelsPerInch;        /* pixels/inch */
int xPixelsPerInch;        /* pixels/inch */


/* Flag if printer setup was done. */
BOOL bPrinterSetupDone=FALSE;
/* 
 * all of these are in device units (for printer) and are calculated
 * by SetupPrinting()
 */

/* Had to use HEAD since HEADER already has another def.  */
#define HEAD   0
#define FOOTER 1

int dyTop;        /* width of top border */
int dyBottom;        /* width of bottom border */
int dxLeft;        /* width of left border */
int dxRight;        /* width of right border (this doesn't get used) */
int dyHeadFoot;        /* height from top/bottom of headers and footers */

int iPageNum;        /* global page number currently being printed */
int xLeftSpace, xRightSpace; /* Space of margins */
short xCharPage;

//FARPROC lpfnAbortProc;
//FARPROC lpfnAbortDlgProc;
//FARPROC lpfnPageDlgProc;

/* We'll dynamically allocate this */
HANDLE hHeadFoot=NULL;
LPSTR  szHeadFoot;

void FreePrintHandles()
{
    if(PD.hDevMode)
        GlobalFree(PD.hDevMode);
    if(PD.hDevNames)
        GlobalFree(PD.hDevNames);
    PD.hDevMode = PD.hDevNames = NULL;
    bPrinterSetupDone = FALSE;
}

/* Get dafault printer data using the commdlg code for printer setup. */
BOOL NEAR GetDefaultPrinter()
{
    FreePrintHandles();
   
    PD.lStructSize     = sizeof(PRINTDLG);
    PD.Flags         = PD_PRINTSETUP|PD_RETURNDEFAULT;
    if (PrintDlg((LPPRINTDLG)&PD))
    {
        bPrinterSetupDone = TRUE;
        return TRUE;
    }
    else
    {
        FreePrintHandles();
        return FALSE;
    }
}

/* Call the commdlg code for printer setup. */
void PrinterSetupDlg(HWND hwnd)
{
    BOOL bTryAgain = (PD.hDevMode || PD.hDevNames);
    DWORD dwErr;

    LockData(0);
    PD.Flags = PD_PRINTSETUP;    /* invoke only the Setup dialog */

TryPrintSetupAgain:
    bPrinterSetupDone |= PrintDlg(&PD);

    /* set szPrinter and free hDevMode and hDevNames */
    /* PrintDlg error. */
    if(dwErr = CommDlgExtendedError()) /* Re-initialize the PD structure. */
    {
        PD.lStructSize    = sizeof(PRINTDLG);
        PD.hwndOwner      = hwnd;
        PD.hDC            = NULL;
        PD.nCopies        = 1;
        FreePrintHandles();
        if (bTryAgain)
          {
            bTryAgain = FALSE;
            goto TryPrintSetupAgain;
          }
        else if (dwErr != PDERR_NODEFAULTPRN)
          {
            char szError[256];

            LoadString(hIndexInstance, E_PRINT_SETUP_ERROR, szError, 256);
            MessageBox(hwnd, szError, szCardfile, MB_OK | MB_ICONHAND);
          }
    }
    UnlockData(0);
}

/*
 * convert floating point strings (like 2.75 1.5 2) into number of pixels
 * given the number of pixels per inch
 */
long atopix(
    char *ptr, 
    int pix_per_in)
{
    char *dot_ptr;
    char sz[20];
    long decimal;

    lstrcpy(sz, ptr);
    dot_ptr = StrChr(sz, szDec[0]);
    if (dot_ptr)
    {
        *dot_ptr++ = 0;        /* terminate the inches */
        if (*(dot_ptr + 1) == 0)
        {
            *(dot_ptr + 1) = '0';   /* convert decimal part to hundredths */
        }
        *(dot_ptr + 2) = 0;
        decimal = (Myatol(dot_ptr) * (long)pix_per_in) / 100;  /* first part */
    }
    else
        decimal = 0L;        /* there is not fraction part */

    return (Myatol(sz) * (long)pix_per_in) + decimal;     /* second part */
}

HDC GetPrinterDC(
    void) 
{
    LPDEVMODE lpDevMode;
    LPDEVNAMES lpDevNames;

    if (!bPrinterSetupDone) /* Retrieve default printer if none selected. */
    { 
        if (!GetDefaultPrinter())
            return NULL;
    }

    lpDevNames  = (LPDEVNAMES)GlobalLock(PD.hDevNames);
    if (PD.hDevMode)
        lpDevMode = (LPDEVMODE)GlobalLock(PD.hDevMode);
    else
        lpDevMode = NULL;
        
    /* For pre 3.0 drivers lpDevMode will be null as these drivers don't
     * use this structure.
     */
    PD.hDC = CreateDC((LPSTR)lpDevNames+lpDevNames->wDriverOffset,
                      (LPSTR)lpDevNames+lpDevNames->wDeviceOffset,
                      (LPSTR)lpDevNames+lpDevNames->wOutputOffset,
                      lpDevMode);
    GlobalUnlock(PD.hDevNames);
    if (PD.hDevMode)
        GlobalUnlock(PD.hDevMode);

    return PD.hDC;
}
    
HDC SetupPrintingFailed(
    HDC hPrintDC)
{
    if (hPrintDC)
        DeleteDC(hPrintDC);
    IndexOkError(ECANTPRINT);
    return NULL;
}

/*
 * setup the printer, return it's DC and create printing (abort)
 * dialog box.
 */
HDC SetupPrinting(
    void)
{
    char    buf[40];
    short   nSpace;
    HDC     hPrintDC;
    TEXTMETRIC Metrics;
    int iErr;

    /* 
     * On second print job, this was getting used
     * before the dialog was created for the second time.
     */
    hAbortDlgWnd = (HWND)NULL;

    if (!(hPrintDC = GetPrinterDC()))
        return SetupPrintingFailed(hPrintDC);

    GetTextMetrics(hPrintDC, &Metrics);
    yPrintChar = Metrics.tmHeight + Metrics.tmExternalLeading;
    ExtPrintLeading = Metrics.tmExternalLeading;
    xPrintChar = Metrics.tmAveCharWidth;    /* character width */
    iTabSize = xPrintChar*8;
    xPrintRes = GetDeviceCaps(hPrintDC, HORZRES);
    yPrintRes = GetDeviceCaps(hPrintDC, VERTRES);
    xPixelsPerInch  = GetDeviceCaps(hPrintDC, LOGPIXELSX);
    yPixelsPerInch  = GetDeviceCaps(hPrintDC, LOGPIXELSY);

    dyHeadFoot = yPixelsPerInch / 2;                  /* 1/2 an inch */
    dyTop      = atopix(chPageText[4], yPixelsPerInch);
    dyBottom   = atopix(chPageText[5], yPixelsPerInch);
    dxLeft     = atopix(chPageText[2], xPixelsPerInch);
    dxRight    = atopix(chPageText[3], xPixelsPerInch);

    nSpace=GetTextExtent(hPrintDC, " ", 1);
    xLeftSpace=dxLeft/nSpace;
    xRightSpace=dxRight/nSpace;

    /* Number of characters between margins */
    xCharPage=(xPrintRes/xPrintChar)-xLeftSpace-xRightSpace;

    /* Allocate memory for the header.footer string.  Will allow any size
     * of paper and still have enough for the string.
     */
    hHeadFoot=GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, (DWORD)xCharPage+2);

    if (!hHeadFoot)
        return SetupPrintingFailed(hPrintDC);

    Escape(hPrintDC, SETABORTPROC, 0, (LPSTR)fnAbortProc, NULL);
    BuildCaption(buf);

    /* Gotta disable the window before doing the start doc so that the user
     * can't quickly do multiple prints. 
     */
    EnableWindow(hIndexWnd, FALSE);

    if ((iErr = Escape(hPrintDC, STARTDOC, lstrlen(buf), buf, NULL)) < 0)
    {
        DeleteDC(hPrintDC);
        EnableWindow(hIndexWnd, TRUE);
        if (iErr == SP_USERABORT)
            SendMessage(hIndexWnd, WM_SETFOCUS, 0, 0);
        else
            IndexOkError(ECANTPRINT);
        return NULL;
    }

    bError = FALSE;
    fAbort = FALSE;
    hAbortDlgWnd = CreateDialog(hIndexInstance, (LPSTR)DTABORTDLG,
                    hIndexWnd, lpfnAbortDlgProc);
    if (!hAbortDlgWnd)
    {
        EnableWindow(hIndexWnd, TRUE);
        return SetupPrintingFailed(hPrintDC);
    }
    return hPrintDC;
}

void FinishPrinting(
    HDC hPrintDC)
{
    if (!fAbort)
    {
        if (!bError)
            Escape(hPrintDC, ENDDOC, 0, (LPSTR)0, (LPSTR)0);
        EnableWindow(hIndexWnd, TRUE);
        DestroyWindow(hAbortDlgWnd);
    }
    DeleteDC(hPrintDC);
    if (hHeadFoot)
        GlobalFree(hHeadFoot);
    hHeadFoot=NULL;
}


/*
 * print out the translated header/footer string in proper position.
 *
 * uses global stuff like xPrintChar, dyHeadFoot...
 */
NOEXPORT void NEAR PrintHeaderFooter(
    HDC hDC, 
    short nHF)
{
    char    buf[80];
    short   len;

    lstrcpy(buf, chPageText[nHF]);

    szHeadFoot= (LPSTR) GlobalLock(hHeadFoot);
    len=TranslateString(buf);

    if (*szHeadFoot)
    {
        if (nHF==HEAD)
        {
            TabbedTextOut(hDC, dxLeft, dyHeadFoot - yPrintChar, szHeadFoot, 
                    len, 1, &iTabSize, dxLeft);
        }
        else
        {
            TabbedTextOut(hDC, dxLeft, yPrintRes-yPrintChar-dyHeadFoot, 
                    szHeadFoot, len, 1, &iTabSize, dxLeft);
        }
    }
    GlobalUnlock(hHeadFoot);
}

void PrintList(
    void)
{
    HDC hPrintDC;
    int curcard;
    int i, y, Start, End;
    int cCardsPerPage;
    LPCARDHEADER Cards;
    int iError;

    hPrintDC = SetupPrinting();
    if (!hPrintDC)
    {
        if (hHeadFoot)
        {
            GlobalFree(hHeadFoot);
            hHeadFoot=NULL;
        }
        return;
    }

    cCardsPerPage = max(1, (yPrintRes - dyTop - dyBottom) / yPrintChar);
    iPageNum = 1;
    Cards = (LPCARDHEADER)GlobalLock(hCards);
    y = dyTop - yPrintChar;

    for (curcard = 0; curcard < cCards; )
    {
        PrintHeaderFooter(hPrintDC, HEAD);

        Start = curcard;
        End = curcard + cCardsPerPage;
        if (End > cCards)
            End = cCards;
        for (i = Start; i < End; ++i, curcard++)
        {
            TabbedTextOut(hPrintDC, dxLeft, y, Cards[i].line, 
                    lstrlen(Cards[i].line), 1, &iTabSize, dxLeft);
            y += yPrintChar;
        }

        PrintHeaderFooter(hPrintDC, FOOTER);
        iPageNum++;

        /* Reset y so printing starts at top again. */
        y = dyTop - yPrintChar;
        iError = Escape(hPrintDC, NEWFRAME, 0, NULL, 0);
        if (iError < 0)
        {
            PrintError(iError);
            break;
        }
        if (fAbort)
            break;
    }
    GlobalUnlock(hCards);
    FinishPrinting(hPrintDC);
}

void PrintCards(
    int count)
{
    HDC          hPrintDC;
    int          curcard;
    int          i;
    int          y;
    int          cCardsPerPage;
    CARDHEADER   CardHead;
    LPCARDHEADER Cards;
    CARD         Card;
    HDC          hMemoryDC;
    HWND         hPrintWnd;
    HANDLE       hOldObject;
    int          fPictureWarning;
    int          iError;

    hPrintWnd = CreateWindow("Edit", NULL,
        WS_CHILD | ES_MULTILINE,
        0, 0, (LINELENGTH*CharFixWidth)+1, CARDLINES*CharFixHeight,
        hIndexWnd, NULL, hIndexInstance, NULL);
    /* Set fixed pitched font to this edit control, sothat the text does
     * not get chopped off at the right when we print, because we use
     * fixed pitched font;
     */
    if(hPrintWnd)
        SendMessage(hPrintWnd, WM_SETFONT, (WPARAM)hFont, (LONG) FALSE);

    if (!hPrintWnd)
    {
        IndexOkError(EINSMEMORY);
        if (hHeadFoot)
        {
            GlobalFree(hHeadFoot);
            hHeadFoot=NULL;
        }
        return;
    }

    if (!(hPrintDC = SetupPrinting()))
    {
        if (hHeadFoot)
        {
            GlobalFree(hHeadFoot);
            hHeadFoot=NULL;
        }
        return;
    }

/* Bug 8320: Added an extra character so the HP LaserJet doesn't overwrite
 * the edge of the card.  Since clipping regions for graphics are based on
 * xPrintCard, this won't hurt them, and TabTextOut() calls are based on
 * the number of characters held in the buffer (LINELENGTH), so they won't
 * walk off the end.
 *   15 January 1992          Clark R. Cyr
 */

    xPrintCard = ((LINELENGTH + 1) * xPrintChar) + 2;
    yPrintCard = (CARDLINES*yPrintChar) + yPrintChar + 1 + 2 + 2;

    hOldObject = SelectObject(hPrintDC, GetStockObject(HOLLOW_BRUSH));

    hMemoryDC = CreateCompatibleDC(hPrintDC);
    fPictureWarning = FALSE;

    if (count == 1)
    {
        iPageNum = 1;
        if (!hMemoryDC && CurCard.lpObject)
            IndexOkError(ENOPICTURES);

        PrintHeaderFooter(hPrintDC, HEAD);
        PrintCurCard(hPrintDC, hMemoryDC, dxLeft, dyTop, &CurCardHead, &CurCard, hEditWnd);
        PrintHeaderFooter(hPrintDC, FOOTER);

        if ((iError = Escape(hPrintDC, NEWFRAME, 0, NULL, 0)) < 0)
            PrintError(iError);
    }
    else
    {
        iPageNum = 1;
        cCardsPerPage = max(1, (yPrintRes - dyTop - dyBottom) / (yPrintCard + yPrintChar));

        for (curcard = 0; curcard < count; )
        {
            PrintHeaderFooter(hPrintDC, HEAD);
            y = dyTop;
            for (i = 0; i < cCardsPerPage && curcard < count; ++i)
            {
                if (curcard != iFirstCard)
                {
                    Cards = (LPCARDHEADER) GlobalLock(hCards);
                    CardHead = Cards[curcard];
                    GlobalUnlock(hCards);
                    if (!ReadCurCardData(&CardHead, &Card, szText))
                        IndexOkError(ECANTPRINTPICT);
                }
                else
                {
                    CardHead = CurCardHead;
                    Card = CurCard;
                    GetWindowText(hEditWnd, szText, CARDTEXTSIZE);
                }
                SetWindowText(hPrintWnd, szText);
                if (!hMemoryDC && Card.lpObject && !fPictureWarning)
                {
                    fPictureWarning++;
                    IndexOkError(ENOPICTURES);
                }
                PrintCurCard(hPrintDC, hMemoryDC, dxLeft, y, &CardHead, &Card, hPrintWnd);

                if (curcard != iFirstCard && Card.lpObject)
                    PicDelete(&Card);

                y += yPrintCard + yPrintChar;
                curcard++;
            }

            PrintHeaderFooter(hPrintDC, FOOTER);

            iPageNum++;

            if ((iError = Escape(hPrintDC, NEWFRAME, 0, NULL, 0)) < 0)
            {
                PrintError(iError);
                break;
            }

            if (fAbort)
                break;
        }
    }

    DestroyWindow(hPrintWnd);
    SelectObject(hPrintDC, hOldObject);
    FinishPrinting(hPrintDC);
    if (hMemoryDC)
        DeleteDC(hMemoryDC);
}

void PrintCurCard(
    HDC hPrintDC,
    HDC hMemoryDC,
    int xPos,
    int yPos,
    PCARDHEADER pCardHead,
    PCARD   pCard,
    HWND    hWnd)
{
    int y;
    HANDLE hOldObject;
    int level;
    int i;
    int cLines;
    char buf[LINELENGTH];
    int cch;

    Rectangle(hPrintDC, max (0,xPos-(xPixelsPerInch/8)), max(0, yPos-(yPixelsPerInch/16)),
        xPos + xPrintCard, yPos + yPrintCard);
    Rectangle(hPrintDC, max (0,xPos-(xPixelsPerInch/8)), yPos + 1 + yPrintChar,
    xPos + xPrintCard, yPos + ExtPrintLeading + yPrintChar);

    SetBkMode(hPrintDC, TRANSPARENT);
    TabbedTextOut(hPrintDC, xPos+1, yPos + 1+(ExtPrintLeading / 2), 
            pCardHead->line, lstrlen(pCardHead->line), 1, &iTabSize, xPos+1);

    if (pCard->lpObject && hMemoryDC)
    {
        HBITMAP hBitmap;
        BITMAP  bm;

        level = SaveDC(hPrintDC);

        IntersectClipRect(hPrintDC, xPos + 1, yPos + yPrintChar+4,
                xPos + xPrintCard-1, yPos + yPrintCard-1);

        /* Get a bitmap to print */
        if (HIWORD(pCard->lpObject))
            hBitmap = MakeObjectCopy(pCard);
        else
            hBitmap = (HBITMAP)LOWORD(pCard->lpObject);

        GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
        hOldObject = SelectObject(hMemoryDC, hBitmap);

        if (!StretchBlt(hPrintDC,
            xPos + (pCard->rcObject.left * xPrintChar) / CharFixWidth,
            yPos + yPrintChar + 4 + (pCard->rcObject.top * yPrintChar) / CharFixHeight,
            (bm.bmWidth * xPrintChar) / CharFixWidth,
            (bm.bmHeight * yPrintChar) / CharFixHeight,
            hMemoryDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY))
        {
            IndexOkError(ECANTPRINTPICT);
        }

        SelectObject(hMemoryDC, hOldObject);
        RestoreDC(hPrintDC, level);

        if (HIWORD(pCard->lpObject))
             DeleteObject(hBitmap);
    }

    /* draw the text */
    y = yPos + yPrintChar + 4 + (ExtPrintLeading / 2);
    cLines = SendMessage(hWnd, EM_GETLINECOUNT, 0, 0L);
    for (i = 0; i < cLines; ++i)
    {
        buf[0] = LINELENGTH;
        buf[1] = 0;
        cch = (int)SendMessage(hWnd, EM_GETLINE, i, (long)(LPSTR)buf);
        TabbedTextOut(hPrintDC, xPos + 1, y, buf, cch,
                1, (LPINT)&iTabSize, xPos + 1);
        y += yPrintChar;
    }
}

int fnAbortProc(
    HDC hPrintDC,        /* what is this useless parameter? */
    int iReserved)      /* and this one? Good question !*/
{
    MSG msg;

    while (!fAbort && PeekMessage(&msg, NULL, NULL, NULL, TRUE))
    if (!hAbortDlgWnd || !IsDialogMessage(hAbortDlgWnd, (LPMSG)&msg))
    {
        TranslateMessage((LPMSG)&msg);
        DispatchMessage((LPMSG)&msg);
    }
    return(!fAbort);
}

HMENU hSysMenu;

/*
 * dialog procedure for the print dialog
 *
 * this allows the user to cancel printing
 */
int fnAbortDlgProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    PSTR pchTmp;

    switch(msg)
    {
        case WM_COMMAND:
            fAbort = TRUE;
            EnableWindow(hIndexWnd, TRUE);
            DestroyWindow(hwnd);
            hAbortDlgWnd = NULL;
            return(TRUE);

        case WM_INITDIALOG:
            hSysMenu = GetSystemMenu(hwnd, FALSE);
            if (CurIFile[0])
                pchTmp = FileFromPath(CurIFile);
            else
                pchTmp = szUntitled;

            SetDlgItemText(hwnd, DTNAME, pchTmp);
            SetFocus(hwnd);
            return(TRUE);

        case WM_INITMENU:
            EnableMenuItem(hSysMenu, SC_CLOSE, MF_GRAYED);
            return(TRUE);
    }
    return(FALSE);
}

void PrintError(
    int iError)
{
    bError = TRUE;
    if (iError & SP_NOTREPORTED)
    {
        switch(iError)
        {
            case -5:
                IndexOkError(EMEMPRINT);
                break;
            case -4:
                IndexOkError(EDISKPRINT);
                break;
            case -3:
            case -2:
                break;
            default:
                IndexOkError(ECANTPRINT);
                break;
        }
    }
}

BOOL Myisdigit(
    char c)
{
    if (c >= '0' && c <= '9')
        return TRUE;
    else
        return FALSE;
}
/***************************************************************************
 * short TranslateString(char *src)
 *
 * purpose:
 *    translate a header/footer strings
 *
 *     supports the following:
 *
 *    &&    insert a & char
 *    &f    current file name or (untitiled)
 *    &d    date in Day Month Year
 *    &t    time
 *    &p    page number
 *    &p+num    set first page number to num
 *
 * params:
 *    IN/OUT    src    this is the string to translate, gets filled with
 *            translate string.  limited by len chars
 *    IN    len    # chars src pts to
 *
 * used by:
 *    Header Footer stuff
 *
 * uses:
 *    lots of c lib stuff
 *
 * restrictions:
 *     this function uses the following global data
 *
 *    iPageNum
 *    text from main window caption
 * NOTE : Resides in _TEXT segment so that it can call C runtimes
 * most of the print code is in _PRINT segment
 *
 ***************************************************************************/

short TranslateString(
    char *src)
{
    extern int iPageNum;
    extern short xCharPage;
    extern LPSTR  szHeadFoot;

    char         letters[15];
    char         chBuff[3][80], buf[80];
    char         *ptr, *dst=buf, *save_src=src;
    int          page;
    short        nAlign=1, foo, nx,
                 nIndex[3];

    nIndex[0]=0;
    nIndex[1]=0;
    nIndex[2]=0;

    LoadString(hIndexInstance, IDS_LETTERS, letters, 15);

    while (*src)   /* look at all of source */
    {
        while (*src && *src != '&')
        {
            if( IsDBCSLeadByte(*src))
            {
                chBuff[nAlign][nIndex[nAlign]]=*src++;
                nIndex[nAlign] += 1;
            }
            chBuff[nAlign][nIndex[nAlign]]=*src++;
            nIndex[nAlign] += 1;
        }

        if (*src == '&')   /* is it the escape char? */
        {
            src++;

            if (*src == letters[0] || *src == letters[1])
            {                      /* &f file name (no path) */

                /* a bit of sleaze... get the caption from
                 * the main window.  search for the '-' and
                 * look two chars beyond, there is the
                 * file name or (untitled) (cute huh?)
                 */

                GetWindowText(hIndexWnd, buf, 80);
                ptr=StrChr(buf, '-') + 2;

                /* Copy to the currently aligned string. */
                lstrcpy((chBuff[nAlign]+nIndex[nAlign]), ptr);

                /* Update insertion position. */
                nIndex[nAlign] += lstrlen(ptr);
            }
            else if (*src == letters[2] || *src == letters[3])
            {                      /* &P or &P+num page */
                src++;
                page = 0;
                if (*src == '+')       /* &p+num case */
                {
                    src++;
                    while (Myisdigit(*src))
                        {
                        /* Convert to int on-the-fly*/
                        page = (10*page)+(unsigned char)(*src)-48;
                        src++;
                        }

                }
                
                wsprintf(buf, "%d", iPageNum+page);
                lstrcpy((chBuff[nAlign]+nIndex[nAlign]), buf);
                nIndex[nAlign] += lstrlen(buf);
                src--;
            }
            else if (*src == letters[4] || *src == letters[5])
            {                      /* &t time */

                GetDateTime(buf, NULL);

                /* extract time */
                StrCpyN(chBuff[nAlign]+nIndex[nAlign], buf, lstrlen(buf));
                nIndex[nAlign] += lstrlen(buf);
            }
            else if (*src == letters[6] || *src == letters[7])
            {                      /* &d date */
                GetDateTime(NULL, buf);
                /* extract day month day */
                StrCpyN(chBuff[nAlign]+nIndex[nAlign], buf, lstrlen(buf));
                nIndex[nAlign] += lstrlen(buf);
            }
            else if (*src == '&')
            {               /* quote a single & */
                chBuff[nAlign][nIndex[nAlign]]='&';
                nIndex[nAlign] += 1;
            }
            /* Set the alignment for whichever has last occured. */
            else if (*src == letters[8] || *src == letters[9])
                nAlign=1;                    /* &c center */
            else if (*src == letters[10] || *src == letters[11])
                nAlign=2;                    /* &r right */
            else if (*src == letters[12] || *src == letters[13])
                nAlign=0;                   /* &d date */
            src++;
        }
    }
    /* Make sure all strings are null-terminated. */
    for (nAlign=0; nAlign<3; nAlign++)
        chBuff[nAlign][nIndex[nAlign]]=0;

    /* Initialize Header/Footer string */
    for (nx=0; nx<xCharPage; nx++)
        *(szHeadFoot+nx)=32;

    /* Copy Left aligned text. */
    for (nx=0; nx < nIndex[0]; nx++)
        *(szHeadFoot+nx)=chBuff[0][nx];

    /* Calculate where the centered text should go. */
    foo=(xCharPage-nIndex[1])/2;
    for (nx=0; nx<nIndex[1]; nx++)
        *(szHeadFoot+foo+nx)=(char)chBuff[1][nx];

    /* Calculate where the right aligned text should go. */
    foo=xCharPage-nIndex[2];
    for (nx=0; nx<nIndex[2]; nx++)
        *(szHeadFoot+foo+nx)=(char)chBuff[2][nx];


    return lstrlen(szHeadFoot);
}

#define CCHDATE         11
#define CCHTIME         12

TIME Time = {
    ':',        /* Separator character for time string */
    "AM",       /* Time string suffix for morning hours */
    "PM",       /* Time string suffix for afternoon */
    FALSE,      /* True if military time, else False */
    FALSE};     /* TRUE if Leading zero for hour */

CARDDATE Date = {
    0,          /* 0 => MDY */
    '/',        /* '/' */
    "d/M/yy",   /* Short Date format */
    0,          /* Century - 1990 vs. 90, lead zeros for day and month */
};

/* ** Get current date and time from dos, and build string showing same.
      String must be formatted according to country info obtained from
      win.ini. */
static void GetDateTime(char *szTime, char *szDate)
{
    extern  char doscl,dosdh,dosah,dosch,dosdl;
    extern int doscx;
    char szMonth[3], szDay[3], szYear[5];
    char szHour[3];
    int fPM = FALSE;

    /* Get Time */
  if (szTime)
    {
      // TODO: replace with GetDateTime call
      dosah = 0x2C;
      DosCall();
      if (dosch > 12)
          fPM = TRUE;
      if (!Time.f24Hr && dosch > 12)
          dosch -= 12;
      if (Time.fLeadZeroHour)
          wsprintf(szHour, "%02d", dosch);
      else
          wsprintf(szHour, "%d", dosch);
        
      wsprintf(szTime, "%s%c%02d%s", (LPSTR)szHour, Time.chSep, doscl, 
                fPM ? (LPSTR)Time.sz2359 : (LPSTR)Time.sz1159);
    }

    /* Get Date */
  if (szDate)
    {
      // TODO: USE GETDATETIME
      dosah = 0x2A;
      DosCall();

      if (Date.Flags & LEADZEROMONTH)
          wsprintf(szMonth, "%02d", dosdh);
      else
          wsprintf(szMonth, "%d", dosdh);
    
      if (Date.Flags & LEADZERODAY)
          wsprintf(szDay, "%02d", dosdl);
      else
          wsprintf(szDay, "%d", dosdl);

      wsprintf(szYear, "%04d", doscx);
      if (!(Date.Flags & CENTURY))
        {
          szYear[0] = szYear[2];
          szYear[1] = szYear[3];
          szYear[2] = 0;
        }

      if (Date.Order == 0)
          wsprintf(szDate, "%s%c%s%c%s", (LPSTR)szMonth, Date.chSep, 
                (LPSTR)szDay, Date.chSep, (LPSTR)szYear);
      else if (Date.Order == 1)
          wsprintf(szDate, "%s%c%s%c%s", (LPSTR)szDay, Date.chSep, 
                (LPSTR)szMonth, Date.chSep, (LPSTR)szYear);
      else
          wsprintf(szDate, "%s%c%s%c%s", (LPSTR)szYear, Date.chSep, 
                (LPSTR)szMonth, Date.chSep, (LPSTR)szDay);
    }
}

