#include "cardfile.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

/* 
 * Forward Search for szSearch 
 * 
 * Start search from the current selection or cursor if it is a new string.
 * Start from one char after the beginning of the current selection if we are
 * repeating a search.
 * If not found, search continues through the text in the consecutive cards.
 * If still not found, search continues from the beginning of the text in the
 * current card. If still not found, then report failure.
 */
void ForwardSearch( void)
{
    int i, index, iCard = iFirstCard;
    int SelStart, SelEnd;
    char *pStart;
    WORD Len;
    char cHold, buf[160];
    char *pMatch, *pLast;
    CARDHEADER CardHead;
    CARD Card;
    LPCARDHEADER Cards;
    HCURSOR hOldCursor;
    DWORD Sel;

    hOldCursor = SetCursor(hWaitCurs);   /* Putup the Hour glass cursor */
    GetWindowText(hEditWnd, szText, CARDTEXTSIZE);
    /* Get current selection */
    Sel = SendMessage(hEditWnd, EM_GETSEL, 0, 0L);
    SelStart = LOWORD(Sel);
    SelEnd = HIWORD(Sel);
    /* if we are repeating a search and there exists a current selection
     * start from one char after the beginning of the selection. */
    if (fRepeatSearch && SelStart != SelEnd) 
#ifdef DBCS
        // if szText+SelStart happens to point to a DBCS leadbyte, 
	// we move forward 2 instead of 1. LiZ -- 5/14/93
        if (IsDBCSLeadByte((BYTE)(*(szText+SelStart))))
            pStart = szText + SelStart + 2;
        else
            pStart = szText + SelStart + 1;
#else
        pStart = szText + SelStart + 1;
#endif
    else /* not repeating a search or no selection exists */
        pStart = szText + SelStart;

    if (fCase)
    {
        StrStr(pStart, szSearch);
    }
    else
    {
        StrStrI(pStart, szSearch);  
    }
    //pMatch = (fCase ? StrStr(pStart, szSearch):
    //                  StrStrI(pStart, szSearch));  
    if (pMatch)
        goto FS_DONE;

    /* Continue search through the text in the consecutive cards. */
    Cards = (LPCARDHEADER) GlobalLock(hCards);
    for (i = 0, iCard = iFirstCard+1; i < cCards-1; i++, iCard++)
    {
        if (iCard >= cCards)
            iCard = 0;
        CardHead = Cards[iCard];

        /* Stupidly and blindly, ReadCurCardData always
         * creates the object after it reads it.  This
         * is unnecessary.
         */
        ReadCurCardData(&CardHead, &Card, szText);
        if (Card.lpObject)
            PicDelete(&Card);

        pMatch = (fCase ? StrStr(szText, szSearch) :
                          StrStrI(szText, szSearch));
        if (pMatch)
        {
            GlobalUnlock(hCards);
            goto FS_DONE;
        }
    }

    GlobalUnlock(hCards);
    /* szText has been destroyed, reget the window text */
    iCard = iFirstCard;
    GetWindowText(hEditWnd, szText, CARDTEXTSIZE);

    /* Continue search from beginning of the text in the current card. */

    /* Null terminate the string at the last character included in the search */
    pLast = szText + SelStart + lstrlen(szSearch);
    if (fRepeatSearch && SelStart != SelEnd)
#ifdef DBCS
    // if pLast happens to be pointing to a lead
    // byte of a DBC, we need to move 2 bytes. LiZ -- 5/14/93
    {
        pLast = szText;
        while( 1 )
        {
            if(IsDBCSLeadByte((BYTE)*pLast))
            {
                if(pLast+2 >= szText + SelStart + lstrlen(szSearch))
                    break;
                pLast+=2;
            }
            else
            {
                if(pLast+1 >= szText + SelStart + lstrlen(szSearch))
                    break;
                pLast++;
            }
        }
     }
#else
        pLast--;
#endif
    Len = lstrlen(szText);
    if ((pLast - szText) > Len)     /* beyond end of buffer? */
        pLast = szText + Len;
    cHold = *pLast;                 /* remember this char */
    *pLast = 0;                     /* replace with NUL */
    pMatch = (fCase ? StrStr(szText, szSearch) :
                      StrStrI(szText, szSearch));
    *pLast = cHold;                 /* restore the char */

FS_DONE:
    SetCursor(hOldCursor);  /* Restore the old cursor */
    if (pMatch)
    {
        if (iCard != iFirstCard && 
            !ScrollCards(hCardWnd, SB_THUMBPOSITION, iCard))
        {
            MessageBox(hIndexWnd, NotEnoughMem, szCardfile, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
            return;
        }
        index = pMatch - szText;
        SendMessage(hEditWnd, EM_SETSEL, 0, MAKELONG(index, index+lstrlen(szSearch)));
    }
    else
    {
        wsprintf(buf, "\"%s\"", (LPSTR)szSearch);
        if (hDlgFind)
          {
            EnableWindow(hDlgFind, FALSE);  /* Disable since it's modeless */
          }
        BuildAndDisplayMsg(ECANTFIND, buf);
        if (hDlgFind)
          {
            EnableWindow(hDlgFind, TRUE);
            SetFocus(hDlgFind);
          }
    }
}


/* 
 * Reverse Search for szSearch 
 * 
 * Start search from start of the current selection if any in the edit control.
 * If not found, search continues through the text in the preceding cards.
 * If still not found, search continues from the end of the text in the
 * current card till the current selection. If still not found, 
 * then report failure.
 */
void ReverseSearch(
    void)
{
    int i, index, iCard = iFirstCard;
    int SelStart, SelEnd;
    char *pStart, *pEnd;
    char buf[160];
    char *pMatch;
    CARDHEADER CardHead;
    CARD Card;
    LPCARDHEADER Cards;
    HCURSOR hOldCursor;
    DWORD Sel;

    hOldCursor = SetCursor(hWaitCurs);   /* Putup the Hour glass cursor */
    GetWindowText(hEditWnd, szText, CARDTEXTSIZE);
    /* Get current selection */
    Sel = SendMessage(hEditWnd, EM_GETSEL, 0, 0L);
    SelStart = LOWORD(Sel);
    SelEnd = HIWORD(Sel);

    /* Compute last char position where we want the string to be found.
     * This position won't be included in the search, so it will find the 
     * previous occurrence */
    pEnd = szText + SelStart;
    pMatch = (fCase ? StrRStr(szText, pEnd, szSearch):
                      StrRStrI(szText, pEnd, szSearch));  
    if (pMatch)
        goto FS_DONE;

    /* Continue search through the text in the consecutive cards. */
    Cards = (LPCARDHEADER) GlobalLock(hCards);
    for (i = 0, iCard = iFirstCard-1; i < cCards-1; i++, iCard--)
    {
        if (iCard < 0)
            iCard = cCards - 1;
        CardHead = Cards[iCard];

        /* Stupidly and blindly, ReadCurCardData always
         * creates the object after it reads it.  This
         * is unnecessary.
         */
        ReadCurCardData(&CardHead, &Card, szText);
        if (Card.lpObject)
            PicDelete(&Card);
        pEnd = szText + lstrlen(szText);
        pMatch = (fCase ? StrRStr(szText, pEnd, szSearch) :
                          StrRStrI(szText, pEnd, szSearch));
        if (pMatch)
        {
            GlobalUnlock(hCards);
            goto FS_DONE;
        }
    }

    GlobalUnlock(hCards);
    /* szText has been destroyed, reget the window text */
    iCard = iFirstCard;
    GetWindowText(hEditWnd, szText, CARDTEXTSIZE);

    /* Continue search from end of the text in the current card, till the 
     * current selection */
#ifdef DBCS
    // if this is a DBCS lead byte, we need to move forward 
    // 2 bytes. LiZ -- 5/14/93
    if (IsDBCSLeadByte((BYTE)(*(szText + SelEnd - lstrlen(szSearch)))))
        pStart = szText + SelEnd - lstrlen(szSearch) + 2;
    else
        pStart = szText + SelEnd - lstrlen(szSearch) + 1;
#else
     pStart = szText + SelEnd - lstrlen(szSearch) + 1;
#endif
    if (pStart < szText)
        pStart = szText;
    pEnd = szText + lstrlen(szText);
    pMatch = (fCase ? StrRStr(pStart, pEnd, szSearch) :
                      StrRStrI(pStart, pEnd, szSearch));
FS_DONE:
    SetCursor(hOldCursor);  /* Restore the old cursor */
    if (pMatch)
    {
        if (iCard != iFirstCard && 
            !ScrollCards(hCardWnd, SB_THUMBPOSITION, iCard))
        {
            MessageBox(hIndexWnd, NotEnoughMem, szCardfile, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
            return;
        }
        index = pMatch - szText;
        SendMessage(hEditWnd, EM_SETSEL, 0, MAKELONG(index, index+lstrlen(szSearch)));
    }
    else
    {
        wsprintf(buf, "\"%s\"", (LPSTR)szSearch);
        EnableWindow(hDlgFind, FALSE);   /* Must disable since it's modeless */
        BuildAndDisplayMsg(ECANTFIND, buf);
        EnableWindow(hDlgFind, TRUE);
        SetFocus(hDlgFind);
    }
}

void DoGoto(
    char *pBuf)
{
    int i;
    int j;
    LPCARDHEADER Cards;
    int iNextFirst;
    char buf[160];

    Cards = (LPCARDHEADER) GlobalLock(hCards);
    for (i = 1, j = iFirstCard+1; i <= cCards; i++, j++)
    {
        if (j >= cCards)
            j = 0;
	iNextFirst = i;
	if (StrStrI(Cards[j].line, pBuf))
            break;
    }
    GlobalUnlock(hCards);

    if (i <= cCards)     /* found it */
    {
        if (CardPhone==CCARDFILE)
		ScrollCards(hCardWnd, SB_THUMBPOSITION, j);
	else
		SendMessage(hListWnd, LB_SETCURSEL, j, 0L);
    }
    else
    {
        wsprintf(buf, "\"%s\"", (LPSTR)pBuf);
        EnableWindow(hDlgFind, FALSE);   /* Must disable since it's modeless */
        BuildAndDisplayMsg(ECANTFIND, buf);
        EnableWindow(hDlgFind, TRUE);
    }
}
