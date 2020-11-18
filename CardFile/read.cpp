#include "cardfile.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 2003 - All Rights Reserved            */
/*                                                                      */
/************************************************************************/

char szRRG[] = "RRG";       /* new cardfile signature.(OLE) */
char szMGC[] = "MGC";       /* old cardfile signature. */

OFSTRUCT ofMainFile;
HANDLE fhMain= INVALID_HANDLE_VALUE;
BOOL fOldCardfile;        /* Are we in the compatibility zone? */


NOEXPORT void NEAR PASCAL LinksExistCheck(
    int fh, 
    int nCardCount,
    BOOL fOld);

NOEXPORT int NEAR AskOkToSaveChanges(
    char *szFile);

NOEXPORT BOOL NEAR SaveChanges(
    char *szFile);

NOEXPORT int NEAR ReadCardFile(
    char *pName);

NOEXPORT BOOL NEAR DoMerge(
    char *szFile);

NOEXPORT int NEAR MergeCardFile(
    char *pName);

BOOL MaybeSaveFile(
    int fSystemModal)
{
    int result;
    char szFile[PATHMAX];

    /* put up a message box that says "Do you wish to save your edits?" */
    /* if so, save 'em */
    /* if returns FALSE, means it couldn't save, and whatever is happening */
    /* should not continue */
    if (!fReadOnly && 
        (fFileDirty || CurCardHead.flags & FDIRTY || SendMessage(hEditWnd, EM_GETMODIFY, 0, 0L)))
    {
        result = AskOkToSaveChanges(szFile);
        if (result == IDYES)
        {
            if (!SaveChanges(szFile))
                return FALSE;           /* didn't save */
        }
        else if (result == IDCANCEL)
            return(FALSE);
        else        /* result == IDNO */
        {
            if (CurCard.lpObject)
                PicDelete(&CurCard);

            if (fOLE && S_OK != OleRevertClientDoc(lhcdoc))
                ErrorMessage(W_FAILED_TO_NOTIFY);
        }
    }
    else if (CurCard.lpObject)
        PicDelete(&CurCard);

    /* Delete "Undo" object before exiting */
    DeleteUndoObject();
    return(TRUE);
}

/* 
 * returns user response for OK to Save cards.
 * returns MB_YES/MB_NO/MB_CANCEL
 */
NOEXPORT int NEAR AskOkToSaveChanges(
    char *szFile)
{
    char szString[100];
    char szMsg[200];
    int result;

    LoadString(hIndexInstance, IOKTOSAVE, szString, 100);

    if (CurIFile[0])
        lstrcpy(szFile, FileFromPath(CurIFile));
    else
        lstrcpy(szFile, szUntitled);
    AnsiUpper(szFile);
    MergeStrings(szString, szFile, szMsg);
    result = MessageBox(hIndexWnd, szMsg, szNote, MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_APPLMODAL);
    
    return result;
}

/* saves changes to a file */
NOEXPORT BOOL NEAR SaveChanges(
    char *szFile)
{
    int fGetName;

    if (!SaveCurrentCard(iFirstCard))
        return FALSE;

    if (CurIFile[0])                /* If CurIFile exists, */
    {                                                                   
        lstrcpy(szFile, CurIFile);  /* use it.             */
        fGetName = FALSE;           /* no need to get a file name */
    }
    else
        fGetName = TRUE;            /* no filename exists, get one */

    /* 
     * Various cases enumerated:
     * If CurIFile is present, use it for saving changes.
     * If CurIFile not present, prompt for a filename.
     * Whenever we successfully write a file we exit the loop.
     *
     * If WriteCardFile fails, prompt for a new filename and try again.
     * MyGetSaveFileName returns FALSE when the SaveFileName dlg is
     * cancelled.
     */
    while (TRUE)
    {
        if (fGetName)               /* get a new filename */
        {
            if (!MyGetSaveFileName(szFile))
            {
                /* SaveFileName dlg was cancelled */
                SetCurCard(iFirstCard);
                return(FALSE);        
            }
        }

        /* save file, if can't save don't continue */
        /* NEVER SAVE AS OLD FORMAT in this case... */
        if (fOLE && S_OK != OleRenameClientDoc(lhcdoc, szFile))
            ErrorMessage(W_FAILED_TO_NOTIFY);

        if (WriteCardFile(szFile, FALSE))
            return TRUE;            /* successfully wrote the file */
        else
            fGetName = TRUE;        /* failed to write the file, try again */
    }
}

void MenuFileOpen(
    void)
{
    char szFile[PATHMAX];

    SaveCurrentCard(iFirstCard);       
    if (MaybeSaveFile(FALSE)) 
    {
        OFN.lpstrDefExt       = (LPSTR)szFileExtension;
        OFN.lpstrFilter       = (LPSTR)szFilterSpec;
        OFN.lpstrCustomFilter = (LPSTR)szCustFilterSpec;
        *szFile = 0;
        OFN.lpstrFile         = (LPSTR)szFile;
        OFN.lpstrInitialDir   = (LPSTR)szLastDir;
        OFN.lpstrTitle        = szOpenCaption;
        OFN.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

        SetCurCard(iFirstCard); /* hold onto the current card */
        LockData(0);
        if (GetOpenFileName (&OFN)) 
            OpenNewFile(szFile);
        else if (CommDlgExtendedError()) /* Assumes low memory. */
            IndexOkError(EINSMEMORY);
        UnlockData(0);
    }
}

int OpenNewFile(
    char szFile[])
{
    /* Register new document */
    if (lhcdoc)
        ReleaseClientDoc();
    if (fOLE && S_OK != OleRegisterClientDoc(szCardfile,
                                   szFile, 0L, &lhcdoc))
    {
        ErrorMessage(W_FAILED_TO_NOTIFY);
        lhcdoc = NULL;
        SendMessage(hIndexWnd, WM_COMMAND, NEW, 0L);
        return FALSE;
    }
    if (DoOpen(szFile))
    {
        SetCurCard(iFirstCard); 
        SetNumOfCards();
        return TRUE;
    }
    else
    {
        SendMessage(hIndexWnd, WM_COMMAND, NEW, 0L);
        return FALSE;
    }
}

int DoOpen( char *szFile)
{
    SetCursor(hWaitCurs);
    if(ReadCardFile(szFile))
    {
        SetCaption();
        Fdelete(TempFile);
        MakeTempFile();
        iTopCard = iFirstCard = 0;
        CurCardHead.flags = 0;
        InvalidateRect(hIndexWnd, NULL, TRUE);
        SetCursor(hArrowCurs);
        return TRUE;
    }
    else
    {
        SetCursor(hArrowCurs);
        return FALSE;
    }
}

/* 
 * Check cardfile signature
 * returns, -1 on error, 
 *          TRUE if it is an old cardfile
 *          FALSE if it is not an old Cardfile
 */
int CheckCardfileSignature(
    int fh)
{
    char Signature[4];
        
    /* MGC is old, RRG is new file */
    Signature[3] = 0;
    myread(fh, Signature, 3);
    if (lstrcmp(Signature, szRRG) == 0)         /* new file? */
    {        
        /* for new files OLE must be present */
        if (fOLE) 
        {
            myread(fh, &idObjectMax, sizeof(DWORD));
            return FALSE;                       /* not an old file */
        } else
            IndexOkError(E_NEW_FILE_NOT_READABLE);  /* error */
    }
    else if (lstrcmp(Signature, szMGC) == 0) 
    {  /* old file? */
        idObjectMax = 0;
        return TRUE;                            /* it is an old file */
    }
    else        /* not a valid cardfile */
    {
        IndexOkError(ENOTVALIDFILE);            /* error */
    }
    return -1;                                  /* report failure */
}

/*
 * read in a file
 */
NOEXPORT int NEAR ReadCardFile(
    char *pName)
{
    int fh;
    LPCARDHEADER Cards;
    WORD i;
    char szFile[PATHMAX];
    WORD cNewCards;
    OFSTRUCT ofMain;
    unsigned long fhHeaderLoc;

    lstrcpy(szFile, pName);
    /* If trying to open the currently open file, close it to avoid sharing 
     * violation */
    if (fhMain >= 0 && lstrcmpi(CurIFile, szFile) == 0)
    {
        CloseHandle(fhMain);
        fhMain = -1;
    }
        
    fReadOnly = FALSE;
    if ((fh = OpenFile(szFile, &ofMain, 
                OF_SHARE_DENY_WRITE | OF_READWRITE | OF_CANCEL)) < 0)
    {
        if ((fh = OpenFile(szFile, &ofMain, 
                OF_READ | OF_CANCEL)) < 0)
            return FALSE;
        fReadOnly = TRUE;
        BuildAndDisplayMsg(W_OPENFILEFORREADONLY, szFile);
    }

    /* check cardfile signature */
    if ((fOldCardfile = CheckCardfileSignature(fh)) < 0)
    {
        CloseHandle(fh);
        return FALSE;
    }

    Hourglass(TRUE);
    /* read the number of cards in the file */
    myread(fh, &cNewCards, sizeof(int));
    if (!ExpandHdrs(cNewCards-cCards) || 
        !(Cards = (LPCARDHEADER) GlobalLock(hCards)))
    {
        CloseHandle(fh);
        Hourglass(FALSE);
        return FALSE;
    }

    /* save first header location */
    if ((fhHeaderLoc = _llseek(fh, 0L, 1)) == -1)
        return FALSE;

    cCards = cNewCards;
    for(i = 0; i < cNewCards; i++) 
    {
        myread(fh, &CurCardHead, sizeof(CARDHEADER));
        Cards[i] = CurCardHead;
    }
    GlobalUnlock(hCards);

    /* go back to first header location */
    if (_llseek(fh, fhHeaderLoc, 0) != -1)
        LinksExistCheck(fh, cCards, fOldCardfile);

    fFileDirty = FALSE;
    ofMainFile = ofMain;     /* save new OFSTRUCT */
    if (fhMain >= 0)        /* If there exists an open file, close it. */
        CloseHandle(fhMain);
    fhMain = fh;
    lstrcpy(CurIFile, szFile);
    Hourglass(FALSE);
    InitPhoneList(hListWnd, iFirstCard);

    return TRUE;
}

void MenuFileMerge(
    void)
{
    char szFile[PATHMAX];

    OFN.lpstrDefExt       = (LPSTR)szFileExtension;
    OFN.lpstrFilter       = (LPSTR)szFilterSpec;
    OFN.lpstrCustomFilter = (LPSTR)szCustFilterSpec;
    *szFile = 0;
    OFN.lpstrFile         = (LPSTR)szFile;
    OFN.lpstrInitialDir   = (LPSTR)szLastDir;
    OFN.lpstrTitle        = szMergeCaption;
    OFN.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

    LockData(0);
    if(!fNoTempFile && GetOpenFileName(&OFN))
    {
        DoMerge(szFile);
        SetNumOfCards();
    }
    UnlockData(0);
}

NOEXPORT BOOL NEAR DoMerge(
    char *szFile)
{
    OFSTRUCT ofStruct;

    if (OpenFile(szFile, (LPOFSTRUCT)&ofStruct, OF_PARSE))
        return IndexOkError(EINVALIDFILE);

    SetCursor(hWaitCurs);
    /* In phone mode no need to save the current card.
     * In card mode should save the current card */
    if (CardPhone == PHONEBOOK || SaveCurrentCard(iFirstCard))
    {
        if(MergeCardFile(szFile))
        {
            iTopCard = iFirstCard = 0;
            if (CardPhone == CCARDFILE)
                SetCurCard(iFirstCard);
            InvalidateRect(hIndexWnd, (LPRECT)NULL, TRUE);
        }
    }
    SetCursor(hArrowCurs);
}

/*
 * read in a file and merge it with the currently open file
 */
NOEXPORT int NEAR MergeCardFile(
    char *pName)
{
    int fh;
    WORD i;
    char szFile[PATHMAX];
    OFSTRUCT ofStruct;
    WORD cMergeCards;
    WORD tSize;
    DWORD oHdr;             /* file offset for the card header */
    BOOL fOld;
    unsigned long fhHeaderLoc;

    /* open file */
    lstrcpy(szFile, pName);
    if ((fh = OpenFile(szFile, &ofStruct, 
                OF_SHARE_DENY_WRITE | OF_READ | OF_CANCEL)) < 0)
        return FALSE;

    if ((fOld = CheckCardfileSignature(fh)) < 0)
    {
        CloseHandle(fh);
        return FALSE;
    }

    /* read the number of cards in the file */
    myread(fh, &cMergeCards, sizeof(int));

    /* save first header location */
    if ((fhHeaderLoc = _llseek(fh, 0L, 1)) == -1)
        return FALSE;

    Hourglass(TRUE);

    if (!ExpandHdrs(cMergeCards))
    {
        CloseHandle(fh);
        Hourglass(FALSE);
        return FALSE;
    }

    fFileDirty = TRUE;
    for (i = 0; i < cMergeCards; ++i)
    {
        myread(fh, &CurCardHead, sizeof(CARDHEADER));

        oHdr = _llseek(fh, 0L, 1);  /* remember offset for the next header */

        /* seek to card data and read it */
        if (oHdr == -1 ||
            _llseek(fh, CurCardHead.lfData, 0) == -1 ||
            !PicRead(&CurCard, fh, fOld) ||     /* read object, */ 
            TextRead(fh, szText) < 0)           /* then text    */
        {
            IndexOkError(E_FAILED_TO_READ_CARD);
            goto end;
        }

        /* Keep adding the current card, to the top */
        if (tSize = WriteCurCard(&CurCardHead, &CurCard, szText))
            iFirstCard = AddCurCard();

        if (CurCard.lpObject)
            PicDelete(&CurCard);

        /* go to the next header */
        _llseek(fh, oHdr, 0);
        if (!tSize)
        {
            fFileDirty = FALSE; /* skip merging */
            goto end;
        }
    }

    /* go back to first header location */
    if (_llseek(fh, fhHeaderLoc, 0) == -1)
        goto end;

    LinksExistCheck(fh, cMergeCards, fOld);

    end:
    Hourglass(FALSE);
    CloseHandle(fh);

    InitPhoneList(hListWnd, iFirstCard);
    return TRUE;
}

NOEXPORT void NEAR PASCAL LinksExistCheck(int fh, int nCardCount, BOOL fOld)
/* assumes fh position is set to first header to read */
{
    CARDHEADER CardHeadTmp;
    unsigned long fhHeaderLoc;
    CARD CardTmp;

    Hourglass(TRUE);

    while(nCardCount--)
    {
        myread(fh, &CardHeadTmp, sizeof(CARDHEADER));

        /* save next header location */
        if ((fhHeaderLoc = _llseek(fh, 0L, 1)) == -1)
            goto end;

        /* goto data location */
        if (_llseek(fh, CardHeadTmp.lfData, 0) == -1) 
            goto end;

        if (!PicRead(&CardTmp, fh, fOld))
            goto end;
        if (CardTmp.lpObject)
        {
            if (CardTmp.otObject == LINK)
            {
                char szMsg[190];
                LoadString(hIndexInstance, IDS_UPDATELINK, szMsg, sizeof(szMsg));
                MessageBox(hIndexWnd, (LPSTR)szMsg, (LPSTR)szCardfile, MB_OK);
                PicDelete(&CardTmp);
                goto end;
            }
            PicDelete(&CardTmp);
        }

        /* goto next header location */
        if (_llseek(fh, fhHeaderLoc, 0) == -1)
            goto end;

    }

    end:
    Hourglass(FALSE);
}
