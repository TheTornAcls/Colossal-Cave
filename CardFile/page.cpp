#include "cardfile.h"

#include <drivinit.h>
#include <cderr.h>

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

/* Page margins values are always stored in inches. 
 * chPageText[ID_LEFT .. ID_BOTTOM] contain the strings used to display the
 * margin values in the dlg box. 
 * When the system of measurement is metric, margins are displayed in CMs,
 * validated in CMs, but are converted to inches before being stored in 
 * chPageText[]. 
 * For precision, arithmetic with margins values is done after multiplying
 * by BASE and them dividing by 100.
 */
NOEXPORT int NEAR CheckMarginNums(
    HWND hWnd);

NOEXPORT BOOL NEAR SetDlgItemNum(
    HWND hDlg, 
    int nItemID, 
    LONG lNum, 
    BOOL bDecimal);

NOEXPORT BOOL NEAR StrToNum(
    LPSTR lpNumStr, 
    LONG FAR *lpNum);

NOEXPORT BOOL NEAR NumToStr(
    LPSTR lpNumStr, 
    LONG lNum, 
    BOOL bDecimal);

/* define a type for NUM and the base */
typedef long NUM;
#define BASE 100L

/* converting in/out of fixed point */
#define  NumToShort(x,s)   (LOWORD(((x) + (s)) / BASE))
#define  NumRemToShort(x)  (LOWORD((x) % BASE))

/* rounding options for NumToShort */
#define  NUMFLOOR      0
#define  NUMROUND      (BASE/2)
#define  NUMCEILING    (BASE-1)

#define  ROUND(x)  NumToShort(x,NUMROUND)
#define  FLOOR(x)  NumToShort(x,NUMFLOOR)

/* Unit conversion */
#define  InchesToCM(x)  (((x) * 254L + 50) / 100)
#define  CMToInches(x)  (((x) * 100L + 127) / 254)


#if 0
// This is stupid!  There is already an atopix() which is exactly the same
// as this!  Don't use this one any more: I put a fix to an overflow problem
// in atopix().  MikeSh 4mar93.

/*
 * convert floating point strings (like 2.75 1.5 2) into number of pixels
 * given the number of pixels per CM
 */
long CMToPixels(
    char *ptr, 
    int pix_per_CM)
{
    char *dot_ptr;
    char sz[20];
    int decimal;

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
        decimal = ((int)Myatol(dot_ptr) * pix_per_CM) / 100;    /* first part */
    }
    else
        decimal = 0;        /* there is not fraction part */

    return (Myatol(sz) * pix_per_CM) + decimal;     /* second part */
}
#endif

/*
 * dialog procedure for page setup
 *
 * this guy sets the global variables that define how printing is too be done
 * (ie margins, headers, footers)
 */
int PageSetupDlgProc(
    HWND hwnd,
    unsigned msg,
    WORD wParam,
    LONG lParam)
{
    int id;         /* ID of dialog edit controls */
    long Num;
    static BOOL fEnglish; 
    /* store the value of system of measurement currently in use. Shouldn't 
     * change when the dlg is up */

    switch (msg)
    {
        case WM_INITDIALOG:
            fEnglish = iMeasure;

            for (id = ID_HEADER; id <= ID_FOOTER; id++)
            {
                SendDlgItemMessage(hwnd, id, EM_LIMITTEXT, PT_LEN-1, 0L);
                SetDlgItemText(hwnd, id, chPageText[id - ID_HEADER]);
            }
            for (id = ID_LEFT; id <= ID_BOTTOM; id++)
            {
                SendDlgItemMessage(hwnd, id, EM_LIMITTEXT, 4, 0L);
                if (!fEnglish)
                {
                    StrToNum(chPageText[id-ID_HEADER], &Num);
                    Num = InchesToCM(Num);
                    SetDlgItemNum(hwnd, id, Num, TRUE);
                }
                else
                    SetDlgItemText(hwnd, id, chPageText[id-ID_HEADER]);
            }
            SendDlgItemMessage(hwnd, ID_HEADER, EM_SETSEL, 0, MAKELONG(0, PT_LEN-1));
            return(TRUE);

        case WM_COMMAND:
            switch (wParam)
            {
                case IDOK:
                    /* Check if margin values are valid. */
                    id = CheckMarginNums(hwnd);
                    if ( id <= 0)   /* invalid */
                    {
                        MessageBox(hwnd, szMarginError, szCardfile, MB_OK | MB_ICONEXCLAMATION);
                        if (id == 0)    /* can't guess which margin is invalid */
                            return TRUE;    /* continue the dialog */
                        else if (id < 0)    /* -id is the ID of the child with invalid value */
                        {
                            SetFocus(GetDlgItem(hwnd, -id));
                            return FALSE;
                        }
                    }
                    /* store the changes made only if valid. */
                    for (id = ID_HEADER; id <= ID_FOOTER; id++)
                        GetDlgItemText(hwnd, id, chPageText[id-ID_HEADER], PT_LEN);
                    for (id = ID_LEFT; id <= ID_BOTTOM; id++)
                    {
                        GetDlgItemText(hwnd, id, chPageText[id-ID_HEADER], PT_LEN);
                        if (!fEnglish)  /* metric system, convert to inches before storing */
                            if (StrToNum(chPageText[id-ID_HEADER], &Num))
                                NumToStr(chPageText[id-ID_HEADER], CMToInches(Num), TRUE);
                    }

                    /* fall through... */

                case IDCANCEL:
                    EndDialog(hwnd, NULL);
                    break;
                }
            return(TRUE);
    }

    return(FALSE);
}

/* Check validity of margin values specified.
 *  return TRUE if margins are valid.
 *
 *  returns  -ID_LEFT if Left margin is invalid, 
 *           -ID_RIGHT if Right margin is invalid
 *           -ID_TOP   if Top margin is invalid
 *           -ID_BOTTOM if Bottom margin is invalid 
 *           FALSE if it cannot guess the invalid margin 
 */
NOEXPORT int NEAR CheckMarginNums(
    HWND hWnd)
{
    short   n;
    char    *pStr;
    char    szStr[PT_LEN];
    long    Left, Right, Top, Bottom;
    HDC  hPrintDC;
    int     xPrintRes, yPrintRes;
    int     xPixelsPer, yPixelsPer;     /* pixels per INCH/CM */

    for (n = ID_HEADER+2; n <= ID_BOTTOM; n++)
    {
        GetDlgItemText(hWnd, n, szStr, PT_LEN);
        pStr = szStr;

        while (*pStr)
        {
            if (Myisdigit(*pStr) || *pStr == szDec[0])
                pStr = (char *)(DWORD)AnsiNext(pStr);
            else
                return (-n);
        }
    }

    if (!(hPrintDC = GetPrinterDC()))
        return TRUE;    /* can't do any range check, assume OK */

    xPrintRes = GetDeviceCaps(hPrintDC, HORZRES);
    yPrintRes = GetDeviceCaps(hPrintDC, VERTRES);
    if (iMeasure == 0)
    {
        int xSizeMM;
        int ySizeMM;

        xSizeMM = GetDeviceCaps(hPrintDC, HORZSIZE);
        ySizeMM = GetDeviceCaps(hPrintDC, VERTSIZE);
        xPixelsPer = (int)(((DWORD)xPrintRes*10)/xSizeMM);
        yPixelsPer = (int)(((DWORD)yPrintRes*10)/ySizeMM);
    }
    else
    {
        xPixelsPer = GetDeviceCaps(hPrintDC, LOGPIXELSX);
        yPixelsPer = GetDeviceCaps(hPrintDC, LOGPIXELSY);
    }

    DeleteDC(hPrintDC);

    /* margin values have int/float values. Do range check */
    GetDlgItemText(hWnd, ID_LEFT, szStr, PT_LEN);
    Left     = atopix(szStr, xPixelsPer);

    GetDlgItemText(hWnd, ID_RIGHT, szStr, PT_LEN);
    Right    = atopix(szStr, xPixelsPer);

    GetDlgItemText(hWnd, ID_TOP, szStr, PT_LEN);
    Top      = atopix(szStr, yPixelsPer);

    GetDlgItemText(hWnd, ID_BOTTOM, szStr, PT_LEN);
    Bottom   = atopix(szStr, yPixelsPer);

    /* try to guess the invalid margin */
    if (Left >= xPrintRes)
        return -ID_LEFT;            /* Left margin is invalid */
    else if (Right >= xPrintRes)
        return -ID_RIGHT;           /* Right margin is invalid */
    else if (Top >= yPrintRes)
        return -ID_TOP;             /* Top margin is invalid */
    else if (Bottom >= yPrintRes)
        return -ID_BOTTOM;          /* Bottom margin is invalid */
    else if (Left >= (xPrintRes-Right))
        return FALSE;                   /* can't guess, return FALSE */
    else if (Top >= (yPrintRes-Bottom))
        return FALSE;                   /* can't guess, return FALSE */
    
    return TRUE;
}


NOEXPORT BOOL NEAR StrToNum(LPSTR lpNumStr, LONG FAR *lpNum)
{
   LPSTR s;
   char szNum[10];
   LONG  lNum = 0;
   BOOL  fSign;

   lstrcpy(szNum, lpNumStr);   
   /* assume we have an invalid number */
   *lpNum = -1;

   /* find the decimal point or EOS */
   for (s = szNum; *s && *s != szDec[0]; ++s)
      ;

   /* add two zeros on end of string */
   lstrcat(szNum, "00");

   /* move decimal point right two places */
   s[3] = '\0';
   if (*s == szDec[0])
      lstrcpy(s, s + 1);

   /* find beginning of number */
   for (s = szNum; *s == ' ' || *s == '\t'; ++s)
      ;

   /* save sign */
   if (*s == '-') {
       fSign = TRUE;
       ++s;
   } else
       fSign = FALSE;

   /* convert the number to a long */
   while (*s) {
      if (*s < '0' || *s > '9')
         return FALSE;

      lNum = lNum * 10 + *s++ - '0';
   }

   /* negate result if we saw negative sign */
   if (fSign)
       lNum = -lNum;

   *lpNum = lNum;
   return TRUE;
}

BOOL GetDlgItemNum(HWND hDlg, int nItemID, LONG FAR * lpNum)
{
   char num[20];

   /* get the edit text */
   if (!GetDlgItemText(hDlg, nItemID, num, 20))
      return FALSE;

   return StrToNum(num, lpNum);
}

NOEXPORT BOOL NEAR NumToStr(LPSTR lpNumStr, LONG lNum, BOOL bDecimal)
{
   if (bDecimal)
      wsprintf(lpNumStr, "%d%c%02d", FLOOR(lNum), szDec[0], NumRemToShort(lNum));
   else
      wsprintf(lpNumStr, "%d", ROUND(lNum));

   return TRUE;
}

NOEXPORT BOOL NEAR SetDlgItemNum(
    HWND hDlg, 
    int nItemID, 
    LONG lNum, 
    BOOL bDecimal)
{
   char  num[20];

   NumToStr(num, lNum, bDecimal);
   SetDlgItemText(hDlg, nItemID, num);

   return TRUE;
}




