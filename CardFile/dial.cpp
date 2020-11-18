/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/
#define USECOMM
#include "cardfile.h"


#define HANGUP_CMD    "ATH0\r"

/*
 * general form of modem string
 *
 * MODEM=COMn,T|P,0|1|2|3|4...
 *
 * COMn == com # (1, 2, 3, 4)
 * TorP == Tone or Pulse
 * 0|1|2|.. == speed number (0==120, 1==300, 2==1200, 3==2400...
 * 
 */

#define PRE_LEN 6
#ifdef KOREA
char PrefixDefault[] = "";
#else
char PrefixDefault[] = "9-";
#endif
char Prefix[PRE_LEN];

char ModemInitDefault[] = "COM1,T,2";    /* com1, tone dialing, 1200 baud */
char ModemInit[15];

char *pcComNum    = ModemInit + 3;    /* ptrs to the stuff we want */
char *pcTonePulse = ModemInit + 5;
char *pcSpeedNum  = ModemInit + 7;


NOEXPORT void NEAR GetPhoneNumber(
    PSTR pchBuf,
    int cchMax);

NOEXPORT BOOL NEAR ParseNumber(
    LPSTR lpSrc,
    char *pchBuf,
    int cchMax);

NOEXPORT void NEAR SetPortState(
    int  cid);

NOEXPORT int NEAR MakeDialCmd(
    char *pBuf,
    int  cchMax,
    PSTR pchNumber);

BOOL fnDial(
    HWND hDB,
    unsigned message,
    WORD wParam,
    LONG lParam)
{
    char lpBuf[45];
    char *pResultBuf;
    int len;
    char PhoneNumber[256];
    char buf[40];
    char cmdBuf[200];
    RECT rect, rect1;

    static BOOL fWriteWinIni = FALSE;
    static BOOL fReadWinIni = TRUE;
    static BOOL fUsePrefix = FALSE;

    switch (message)
    {
        case WM_INITDIALOG:
            EnableWindow(GetDlgItem(hDB, ID_SETUP), TRUE);    /* make sure he is on */

            /* resize myself to hold just the number and buttons */
            GetWindowRect(GetDlgItem(hDB, ID_BOX1), &rect);
            GetWindowRect(hDB, &rect1);
            MoveWindow(hDB, rect1.left, rect1.top,
                rect1.right-rect1.left, rect.bottom-rect1.top, FALSE);

            /* do first time initilazation? */
            if (fReadWinIni) {
                GetProfileString(szWindows, "Modem", ModemInitDefault, ModemInit, 15);
                GetProfileString(szWindows, "Prefix", PrefixDefault, Prefix, PRE_LEN-1);
                fUsePrefix = GetProfileInt(szWindows, "UsePrefix", fUsePrefix);

                if (*pcSpeedNum == 'F') {    /* for backwards compatability */
                    *pcSpeedNum = '2';
                    fWriteWinIni = TRUE;
                }

                fReadWinIni = FALSE;
            }

            /* set phone number text and select it */
            GetPhoneNumber(PhoneNumber, 256);
            SetDlgItemText(hDB, ID_NUM, PhoneNumber);
            SendMessage(GetDlgItem(hDB, ID_NUM), EM_SETSEL, 0, (long)lstrlen(PhoneNumber));

            /* set the prefix text */
            SetDlgItemText(hDB, ID_PREFIX, Prefix);

            /* check the use prefix box */
            SendMessage(GetDlgItem(hDB, ID_USEPREFIX), BM_SETCHECK, fUsePrefix, 0L);

            /* check the rest of the buttons... */
            CheckRadioButton(hDB, RB_TONE, RB_PULSE, (*pcTonePulse == 'T') ? RB_TONE : RB_PULSE);
            CheckRadioButton(hDB, RB_COM1, RB_COM4, RB_COM1 + (*pcComNum - '1'));
            CheckRadioButton(hDB, RB_110,  RB_19200, RB_110 + (*pcSpeedNum - '0'));

            return(TRUE);

        case WM_COMMAND:
            pResultBuf = NULL;

            switch (wParam)
            {
                case IDOK:              /* DIAL it! */
                    if (fWriteWinIni)
                    {
                        WriteProfileString(szWindows, "Modem", ModemInit);
                        WriteProfileString(szWindows, "Prefix", Prefix);
                        if (fUsePrefix)
                            WriteProfileString(szWindows, "UsePrefix", "1");
                        else
                            WriteProfileString(szWindows, "UsePrefix", "0");
                        fWriteWinIni = FALSE;
                    }

                    /* check for valid number. */

                    /* Get the length.  If 0, do nothing.*/
                    len=GetDlgItemText(hDB, ID_NUM, lpBuf, 40);
                    if (len!=0)
                    {
                        /* Check for a valid number, give error if not valid */
                        if (!ParseNumber(lpBuf, buf, 40))
                        {
                            LoadString(hIndexInstance, ECANTDIAL,
                                                       cmdBuf, sizeof(cmdBuf));
                            MessageBox(hDB, cmdBuf, szCardfile, MB_OK | MB_ICONEXCLAMATION);
                        }
                        else
                        {
                            /* note: this Alloc gets freeded by the caller */
                            if (pResultBuf = (char *)LocalAlloc(LPTR, 40))
                            {
                                if (fUsePrefix)
                                    len = GetDlgItemText(hDB, ID_PREFIX,
                                                     pResultBuf, PRE_LEN-1);
                                else
                                    len = 0;

                                GetDlgItemText(hDB, ID_NUM, pResultBuf + len, 30);
                            }
                        }
                    }
                /* fall through... */

                case IDCANCEL:
                    EndDialog(hDB, (int)pResultBuf);
                    break;

                case ID_SETUP:
                    /* resize myself to fit in the setup controls */
                    SetFocus(GetDlgItem(hDB, ((*pcTonePulse == 'T') ? RB_TONE : RB_PULSE)));
                    EnableWindow(GetDlgItem(hDB, ID_SETUP), FALSE);
                    GetWindowRect(GetDlgItem(hDB, ID_BOX2), &rect);
                    GetWindowRect(hDB, &rect1);
                    MoveWindow(hDB, rect1.left, rect1.top,
                        rect1.right-rect1.left, rect.bottom-rect1.top, TRUE);
                    break;

                case ID_PREFIX:
                    if (HIWORD(lParam) == EN_CHANGE) {
                        GetDlgItemText(hDB, ID_PREFIX, Prefix, PRE_LEN-1);
                        fWriteWinIni = TRUE;
                    }
                    break;

                case ID_USEPREFIX:
                    fUsePrefix = !fUsePrefix;
                    SendMessage(GetDlgItem(hDB, ID_USEPREFIX), BM_SETCHECK, fUsePrefix, 0L);
                    break;

                case RB_TONE:
                case RB_PULSE:
                    CheckRadioButton(hDB, RB_TONE, RB_PULSE, wParam);
                    *pcTonePulse = ((wParam == RB_TONE) ? 'T' : 'P');
                    fWriteWinIni = TRUE;
                    break;

                case RB_COM1:
                case RB_COM2:
                case RB_COM3:
                case RB_COM4:
                    CheckRadioButton(hDB, RB_COM1, RB_COM4, wParam);
                    *pcComNum = (wParam - RB_COM1) + '1';
                    fWriteWinIni = TRUE;
                    break;

                case RB_110:
                case RB_300:
                case RB_1200:
                case RB_2400:
                case RB_4800:
                case RB_9600:
                case RB_19200:
                    CheckRadioButton(hDB, RB_110, RB_19200, wParam);
                    *pcSpeedNum = (wParam - RB_110) + '0';
                    fWriteWinIni = TRUE;
                    break;

                default:
                    return(FALSE);
            }
            return(TRUE);
    
        default:
            return(FALSE);  
    }
}

/***************************************************************************
 * GetPhoneNumber(pchBuf, cchMax)
 *
 * purpose:
 *    look for phone numbers in:
 *    1) the header line of the card
 *    2) the text of the card itself, starting first with the selection
 *
 * params:
 *    IN    cchMax    limit pchBuf to this # chars
 *    OUT    pchBuf    gets the dialing string if
 *
 * used by:
 *    this is called by the dailing dialog function
 *
 * uses:
 *    ParseNumber() to find phone numbers
 *
 * returns:
 *    nothing of interest
 *
 ***************************************************************************/
NOEXPORT void NEAR GetPhoneNumber(
    PSTR pchBuf,
    int cchMax)
{
    LPCARDHEADER lpCard;
    int fFound = FALSE;
    unsigned long lSelection;

    lSelection = SendMessage(hEditWnd, EM_GETSEL, 0, 0L);

    /* first look in card header */
    if (HIWORD(lSelection) == LOWORD(lSelection))
    {
        /* no selection search card */
        lpCard = (LPCARDHEADER) GlobalLock(hCards) + iFirstCard;
        fFound = ParseNumber(lpCard->line, pchBuf, cchMax);
        GlobalUnlock(hCards);
    }

    /* now look in the text of the card, first at the selection */
    if (!fFound)
    {
        GetWindowText(hEditWnd, szText, CARDTEXTSIZE);
        if (HIWORD(lSelection) != LOWORD(lSelection))
        {
            lstrcpy(szText, szText+LOWORD(lSelection));
            *(szText + (HIWORD(lSelection) - LOWORD(lSelection))) = 0;
        }
        fFound = ParseNumber(szText, pchBuf, cchMax);
    }

    if (!fFound)
        *pchBuf = 0;
}

/***************************************************************************
 * BOOL ParseNumber(lpSrc, pchBuf, cchMax)
 *
 * purpose:
 *    look for phone number strings in lpSrc (return the first one found)
 *
 * params:
 *    IN    lpSrc    string to search for numbers
 *    OUT   pchBuf   place to put result
 *    IN    cchMax   limit # chars in pchBuf
 *
 * used by:
 *      GetPhoneNumber(), and from Autodial Dialog box.
 *
 * uses:
 *
 * returns:
 *    BOOL    TRUE if something found, FALSE otherwise
 *
 * restrictions:
 *    as of now this is a very stupid function. a valid number has to
 *    be 4 chars min for example.  a number matching scheme should be
 *    implemented that looks for things like ###-#### (###) ###-#### or
 *    ####.  Also this function returns the first one found.  It should
 *    probally enumerate all that are found.
 *
 ***************************************************************************/
NOEXPORT BOOL NEAR ParseNumber(
    LPSTR lpSrc,
    char *pchBuf,
    int cchMax)
{
    LPSTR lpchTmp;
    LPSTR lpchEnd;
    PSTR pchTmp;
    int fValid;
    char ch;

#ifdef DBCS
    for (lpchTmp = lpSrc; *lpchTmp; lpchTmp=AnsiNext(lpchTmp))
#else
    for (lpchTmp = lpSrc; *lpchTmp; lpchTmp++)
#endif
    {
        pchTmp = pchBuf;
        lpchEnd = lpchTmp;
        while((pchTmp - pchBuf) < cchMax)
        {
            ch = *lpchEnd++;
            if (ch == '-')
            {
                fValid = TRUE;
                *pchTmp++ = ch;
            }
            else
            {
                if ((ch >= '0' && ch <= '9') || ch == '@' ||
                     ch == ',' || ch == '('  || ch == ')' ||
                     ch == '*' || ch == '#')
                {
                    *pchTmp++ = ch;
                }
                /* Allow a space after an area code in parens. */
                else if (!(ch==' ' && *(pchTmp-1)==')' ))
                {
                    *pchTmp = 0;
                    break;
                }
            }
        }

        if (fValid && ((pchTmp - pchBuf) > 3))    /* allow 4 digit numbers */
            return(TRUE);
    }
    return(FALSE);
}

/*
 * dial the phone with the phone number in pchNumber
 */
void DoDial(
    PSTR pchNumber)
{
    int cid;
    char szComm[5];
    char cmdBuf[200];
    int cch;
    COMSTAT ComStatInfo;
    long oldtime;
    HCURSOR hPrevCursor;

    lstrcpy(szComm, "COMx");

    szComm[3] = *pcComNum;

    cid = OpenComm(szComm, 256, 256);
    if (cid >= 0)
    {
        SetPortState(cid);
        GetCommError(cid, &ComStatInfo);

        /* Dial the number */
        cch = MakeDialCmd(cmdBuf, 80, pchNumber);
        while (WriteComm(cid, cmdBuf, cch) <= 0)
        {
            GetCommError(cid, &ComStatInfo);
            FlushComm(cid, 0);
            FlushComm(cid, 1);
        }

        /* wait for dialing to complete */
        /* Set the HourGlass cursor while waiting */
        /* Fix for Bug #6402 --SANKAR-- 12-1-89 */
        hPrevCursor = SetCursor(hWaitCurs);

        oldtime = GetCurrentTime();
        for(;;)
        {
            GetCommError(cid, &ComStatInfo);
            if (!ComStatInfo.cbOutQue)
               break;
            if (GetCurrentTime() - oldtime > 30000)    /* 30 seconds */
            {
                /* Restore the original cursor shape */
                SetCursor(hPrevCursor);
                IndexOkError(ENOMODEM);
                goto DoneDialing;
            }
        }
        /* Restore the original cursor shape */
        SetCursor(hPrevCursor);

        FlushComm(cid, 1);
        LoadString(hIndexInstance, IPICKUPPHONE, cmdBuf, sizeof(cmdBuf));
        MessageBox(hIndexWnd, cmdBuf, szCardfile, MB_OK);

        lstrcpy(cmdBuf, HANGUP_CMD);        /* send hangup command */
        while(WriteComm(cid, cmdBuf, sizeof(HANGUP_CMD)) <= 0)
        {
            GetCommError(cid, &ComStatInfo);
            FlushComm(cid, 0);
            FlushComm(cid, 1);
        }
        while(TRUE)
        {
            GetCommError(cid, &ComStatInfo);
            if (!ComStatInfo.cbOutQue)
                break;
        }

DoneDialing:
        CloseComm(cid);        /* if using DTR this should hangup as well */
    }
    else
    {
        LoadString(hIndexInstance, ECANTDIAL, cmdBuf, sizeof(cmdBuf));
        MessageBox(hIndexWnd, cmdBuf, szCardfile, MB_OK | MB_ICONEXCLAMATION);
    }
}

NOEXPORT void NEAR SetPortState(
    int  cid)
{
    DCB dcb;
    char szPortInfo[30];
    char *pch;
    char szPort[6];

    if (GetCommState(cid, &dcb)!=-1)
    {
        switch (*pcSpeedNum) 
        {
            case '0':
                dcb.BaudRate = 110;
                break;

            case '1':
                dcb.BaudRate = 300;
                break;

            case '2':
                dcb.BaudRate = 1200;
                break;

            case '3':
                dcb.BaudRate = 2400;
                break;

            case '4':
                dcb.BaudRate = 4800;
                break;

            case '5':
                dcb.BaudRate = 9600;
                break;

            case '6':
                dcb.BaudRate = 19200;
                break;
        }

        lstrcpy(szPort, "COMx:");
        szPort[3] = '1' + cid;

        GetProfileString("Ports", szPort, "300,n,8,1", szPortInfo, 30);
        for (pch = szPortInfo; *pch && *pch != ','; ++pch)
            ;
        while(*pch == ',' || *pch == ' ')
            pch++;
        dcb.Parity = *pch == 'n' ? NOPARITY : (*pch == 'o' ? ODDPARITY : EVENPARITY);
        if (*pch)
            pch++;
        while(*pch == ',' || *pch == ' ')
            pch++;
        dcb.ByteSize = *pch == '8' ? 8 : 7;
        if (*pch)
            pch++;
        while (*pch == ',' || *pch == ' ')
            pch++;
        dcb.StopBits = *pch == '2' ? 2 : 0;
        
        dcb.fDtrDisable = FALSE;    /* use DTR for hangup */

        SetCommState(&dcb);
    }
}

/*
 * Create a string for dialing Hayes compatable modems.
 *
 * in:
 *    cchMax - max number of chars to stuff into pBuf
 *    pchNumber - number string to build dialing stuff out of
 *
 * out:
 *    pBuf - output buffer
 *
 * returns:
 *    the length of the command built
 */
NOEXPORT int NEAR MakeDialCmd(
    char *pBuf,
    int  cchMax,
    PSTR pchNumber)
{
    PSTR pch1;
    PSTR pch2;
    int  cb;
    char szCmd[80];        /* build it here */
    char ch;

    lstrcpy(szCmd, "ATDx");    /* dialing prefix */
    szCmd[3] = *pcTonePulse;    /* pulse or tone dialing */

    pch2 = szCmd + 4;        /* use this to fill the string */

    for (pch1 = pchNumber; ch = *pch1++; ) 
    {
        /* copy only these characters */
        if ((ch >= '0' && ch <= '9') || (ch == ',') || (ch == '#') || (ch == '*'))
            *pch2++ = ch;
        else if (ch == '@')    /* delay */
        {
            *pch2++ = ',';
            *pch2++ = ',';
            *pch2++ = ',';
        }
        else if (ch == 'P' || ch == 'T') /* manual switch to Tone or Pulse */
        {
            *pch2++ = 'D';
            *pch2++ = ch;
        }
    }

    *pch2++ = ';';        /* terminate the string */
    *pch2++ = 0x0d;
    *pch2 = 0;

    cb = lstrlen(szCmd);
    if (cchMax < pch2 - szCmd)
    {
        szCmd[cchMax] = 0;
        cb = cchMax;
    }

    lstrcpy(pBuf, szCmd);
    return cb;
}
