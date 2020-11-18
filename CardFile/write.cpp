#include "cardfile.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

int fReadOnly = FALSE;
/* win.ini string for fValidate value */
char szValidateFileWrite[] = "ValidateFileWrite";   
BOOL fValidate = TRUE;  /* TRUE if validating on save */

NOEXPORT BOOL NEAR ValidCardFile(
                                char szFile[]);

NOEXPORT BOOL FileHasTextOnly(
                             HANDLE hCards,
                             int fhMain, 
                             int fhTemp);
/* 
 * Calls GetSaveFileName(), the COMMDLG function.
 * If the filename returned is invalid, it prompts again.
 * If the given filename exists, it asks for overwrite permission.
 * return FALSE if dlg cancelled, TRUE otherwise.
 */

BOOL MyGetSaveFileName( char *szFile)
{
    BOOL fResult;
    HANDLE fh;
    OFSTRUCT ofStruct;

    while (TRUE)
    {
        OFN.lpstrDefExt       = (LPSTR)szFileExtension;
        OFN.lpstrFilter       = (LPSTR)szFilterSpec;
        OFN.lpstrCustomFilter = (LPSTR)szCustFilterSpec;
        lstrcpy(szFile, FileFromPath(CurIFile));
        OFN.lpstrFile         = (LPSTR)szFile;
        OFN.lpstrInitialDir   = (LPSTR)szLastDir;
        OFN.lpstrTitle        = (LPSTR)szSaveCaption;
        OFN.lpfnHook          = MakeProcInstance(HookProc, hIndexInstance);
        OFN.Flags             = OFN_PATHMUSTEXIST | OFN_ENABLEHOOK|OFN_OVERWRITEPROMPT;

        fValidDB = fValidate;
        fInSaveAsDlg = TRUE;
        LockData(0);
        fResult = GetSaveFileName(&OFN);
        UnlockData(0);
        fInSaveAsDlg = FALSE;
        FreeProcInstance(OFN.lpfnHook);

        if (!fResult)
            return FALSE;               /* SaveFile Dlg cancelled */

        fValidate = fValidDB;

        /* New name is same as current file name, share won't let us open 
         * the file again, so we shouldn't even try an open */
        if (lstrcmpi(szFile, CurIFile) == 0)
            return TRUE;

        /* Bug #15611: Test for existance after comparing filename to avoid
         * sharing violation.  3 November 1991  Clark R. Cyr
         */
        if (OpenFile(szFile, &ofStruct, OF_EXIST) == -1)
            return TRUE;        /* file does not exist, return filename */

        fh = OpenFile(szFile, &ofStruct, OF_READWRITE);
        if (fh == -1)   /* file exists and it is read only */
        {
            BuildAndDisplayMsg(E_FILECANTWRITE, szFile);
            continue;
        }
        CloseHandle(fh);
        return TRUE;    /* OK to overwrite, return filename */
    }
}

NOEXPORT BOOL NEAR WriteCardfileFailed(
                                      int errorID,
                                      int fhDest, 
                                      int fhTemp, 
                                      char *DestFile, 
                                      LPCARDHEADER Cards)
{
    Hourglass(FALSE);

    CloseHandle(fhDest);
    CloseHandle(fhTemp);
    if (Cards)
        GlobalUnlock(hCards);
    Fdelete(DestFile);
    return IndexOkError(errorID);
}

int WriteCardFile(
                 char *pName,        
                 BOOL fOldFormat)    /* Save old monochrome card file? */
{
    WORD i;
    int tSize;
    int fSameFile;
    CARDHEADER CardHeader;
    BOOL fOld;

    char szFile[PATHMAX];

    LPCARDHEADER Cards=NULL;
    int fh;

    /* destination file vars */
    char DestFile[PATHMAX];
    OFSTRUCT ofDest;
    int fhDest;
    DWORD oHdr;         /* offset to Hdr in fhDest */
    DWORD oCard;        /* offset to Card in fhDest */

    int fhTemp;         /* temp file containing modified cards */

    lstrcpy(szFile, pName);
    /* 
     * Set destination file name.
     * If saving to source file, an intermediate temp file is needed
     * else can save directly to the given file. 
     */
    if (fSameFile = !lstrcmp(szFile, CurIFile))
    {
        if (!GetTempFileName((BYTE)(CurIFile[0] | TF_FORCEDRIVE), szFileExtension, 0, DestFile))
            return BuildAndDisplayMsg(ECANTMAKEFILE, szFile);
        OemToAnsi(DestFile, DestFile);
    }
    else
        lstrcpy(DestFile, szFile);

    /* Open Destination file */
    fhDest = OpenFile(DestFile, (LPOFSTRUCT)&ofDest, 
                      OF_SHARE_DENY_WRITE | OF_CREATE);
    /* Open the temp file containing modified/new cards */
    fhTemp = OpenFile(TempFile, &ofTempFile, 
                      OF_SHARE_EXCLUSIVE | OF_REOPEN | OF_READWRITE | OF_CANCEL);
    if (fhDest == -1 || fhTemp == -1)
    {
        char szFormat[100];
        char szMsg[200];

        /* close valid file handles */
        if (fhTemp != -1)
            CloseHandle(fhTemp);
        if (fhDest != -1)
            CloseHandle(fhDest);
        LoadString(hIndexInstance, E_FILESAVE, szFormat, 100);
        wsprintf(szMsg, szFormat, (LPSTR)szFile, 
                 (LPSTR)((fhDest == -1)? DestFile : TempFile));
        MessageBox(hIndexWnd, szMsg, szNote, MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }

    if ((fSameFile && fReadOnly) || /* saving to same file which was read in as read only */
        fNoTempFile ||              /* no temp file ? */
        _llseek(fhDest, 0L, 0) == -1)   /* truncate destination file */
    {
        return BuildAndDisplayMsg(ECANTMAKEFILE, szFile);
    }

    Hourglass(TRUE);
    mywrite(fhDest, "", 0);
    if (!fOLE && !fOldFormat)
    {
        /* Write old if the ole library isn't around (real mode) */
        IndexOkError(W_SAVING_OLD_FORMAT);
        fOldFormat = TRUE;
    }

    /* If File has no pictures save it in 3.0 format. This way it can be read
     * by 3.0 and 3.1 cardfile */
    if (FileHasTextOnly(hCards, fhMain, fhTemp))
        fOldFormat = TRUE;

    /* Write cardfile identifier(MGC is old, RRG is new) and 
     * number of cards */
    if (mywrite(fhDest, (fOldFormat ? szMGC : szRRG), 3) < 3)
        return WriteCardfileFailed(EDISKFULLFILE, fhDest, fhTemp, DestFile, Cards);

    if (!fOldFormat && mywrite(fhDest, &idObjectMax, sizeof(DWORD)) < sizeof(DWORD))
        return WriteCardfileFailed(EDISKFULLFILE, fhDest, fhTemp, DestFile, Cards);

    if (mywrite(fhDest, &cCards, sizeof(int)) < sizeof(int))
        return WriteCardfileFailed(EDISKFULLFILE, fhDest, fhTemp, DestFile, Cards);

    oCard = _llseek(fhDest, 0L, 1) +
            (DWORD)((WORD)cCards * (WORD)sizeof(CARDHEADER));

    /* lock down the card headers */
    Cards = (LPCARDHEADER) GlobalLock(hCards);
    for (i = 0; i < cCards; i++)
    {
        /* write out card hdr */
        CardHeader = Cards[i];
        CardHeader.flags &= (~FTMPFILE);    /* no more in the temp file */
        CardHeader.lfData = oCard;          /* set new offset */
        if (mylwrite(fhDest, &CardHeader, sizeof(CARDHEADER)) < sizeof(CARDHEADER))
            return WriteCardfileFailed(EDISKFULLFILE, fhDest, fhTemp, DestFile, Cards);
        oHdr = _llseek(fhDest, 0L, 1);      /* store offset for the next hdr */

        /* pick up card data from src or temp file */
        if (Cards[i].flags & FTMPFILE)
            fh = fhTemp;
        else
            fh = fhMain;

        /* If we're in the compatibility zone, punt */
        fOld = (!(Cards[i].flags & FTMPFILE) && fOldCardfile);
        /* read card data and write it out */
        if (_llseek(fh, Cards[i].lfData, 0) == -1 ||
            !PicRead(&CurCard, fh, !fOLE || fOld))
        {
            if ((GetDriveType(CurIFile[0]  - 'A') == DRIVE_REMOVABLE))
            {
                return WriteCardfileFailed( E_FLOPPY_WITH_SOURCE_REMOVED,
                                            fhDest, 
                                            fhTemp, 
                                            DestFile, 
                                            Cards);
            }
            else
            {
                return WriteCardfileFailed( E_FAILED_TO_READ_CARD,
                                            fhDest, 
                                            fhTemp, 
                                            DestFile, 
                                            Cards);
            }

        }

        if (_llseek(fhDest, oCard, 0) == -1 ||
            !PicWrite(&CurCard, fhDest, fOldFormat))
        {
            return WriteCardfileFailed(EDISKFULLFILE, fhDest, fhTemp, DestFile, Cards);
        }
        if (CurCard.lpObject)
            PicDelete(&CurCard);

        /* read text size and text, write it out */
        if ((tSize = TextRead(fh, szText)) < 0 ||               
            (mywrite(fhDest, &tSize, sizeof(int)) < sizeof(int)) ||
            (mylwrite(fhDest, szText, tSize) < tSize))
        {
            return WriteCardfileFailed(EDISKFULLFILE, fhDest, fhTemp, DestFile, Cards);
        }

        oCard = _llseek(fhDest, 0L, 1); /* store offset for the next card data */
        _llseek(fhDest, oHdr, 0);       /* ready to read the next hdr */
    }

    _llseek(fhDest, 3, 0);       /* put in correct value of idObjectMax */
    if (!fOldFormat && mywrite(fhDest, &idObjectMax, sizeof(DWORD)) < sizeof(DWORD))
    {
        return WriteCardfileFailed(EDISKFULLFILE, fhDest, fhTemp, DestFile, Cards);
    }

    CloseHandle(fhDest);                    /* close destination file */

    /* check if destination file is a valid cardfile */
    /* Cardfile corruption problem:
     * Cardfile occasionally writes out corrupted files. 
     * During File.Save:
     * If the file is corrupted then the rename is skipped, preserving the 
     * original source file from being updated to a corrupted one.
     *
     * During File.SaveAs:
     * If it is corrupted, the file is deleted.
     */
    if (!ValidCardFile(DestFile))
    {
        Fdelete(DestFile);   /* no use creating a corrupted file */
        if (fSameFile)
            BuildAndDisplayMsg(E_FILEUPDATEFAILED, CurIFile);
        else
            BuildAndDisplayMsg(E_FILEWRITEFAILED, DestFile);
        Hourglass(FALSE);
        GlobalUnlock(hCards);
        return TRUE;          /* avoid putting up the SaveAs dialog */
    }

    if (fhMain >= 0)                     /* close source file if any */
    {
        CloseHandle(fhMain);
    }
    if (fSameFile)
    {
        char HackBuffer[PATHMAX];
        char HackDest[PATHMAX];

        CloseHandle(fhTemp);

        AnsiToOem(DestFile, HackDest);
        AnsiToOem(szFile, HackBuffer);
        if (Fdelete(HackBuffer))
        {
            Fdelete(HackDest);  /* remove temp file */
            BuildAndDisplayMsg(E_FILEWRITEFAILED, CurIFile); /* delete old src file */
            fhMain = OpenFile(CurIFile, (LPOFSTRUCT)&ofMainFile, 
                              OF_SHARE_DENY_WRITE | OF_READ | OF_CANCEL);
            return FALSE;
        }
        Frename(HackDest, HackBuffer);  /* temp file is the new src file */
    }
    else
    {
        _llseek(fhTemp, 0L, 0);             /* truncate temp file and close it */
        mywrite(fhTemp, "", 0);
        CloseHandle(fhTemp);
    }

    fReadOnly = FALSE;  /* we just wrote out the new file */
    fhMain = OpenFile(szFile, (LPOFSTRUCT)&ofMainFile, 
                      OF_SHARE_DENY_WRITE | OF_READ | OF_CANCEL);
    lstrcpy(CurIFile, szFile);

    /* check cardfile signature */
    if ((fOldCardfile = CheckCardfileSignature(fhMain)) < 0)
    {
        CloseHandle(fhMain);
        fhMain = -1;
        return FALSE;
    }
    /* read the number of cards in the file */
    myread(fhMain, &cCards, sizeof(int));
    for (i = 0; i < cCards; ++i)
    {
        myread(fhMain, &CardHeader, sizeof(CARDHEADER));
        Cards[i] = CardHeader;
    }
    fFileDirty = FALSE;

    GlobalUnlock(hCards);
    Hourglass(FALSE);

    return TRUE;
}

NOEXPORT BOOL NEAR ValidCardfileFailed( HANDLE fh)
{
    CloseHandle(fh);
    return FALSE;
}

/* 
 * Returns TRUE if PicRead is successful on all the cards in the file 
 * FALSE otherwise.
 */
NOEXPORT BOOL NEAR ValidCardFile( char szFile[])
{
    BOOL fOld;
    int i;
    HANDLE fh;
    char Signature[4];
    WORD nCards;
    DWORD nObjectMax;
    DWORD cBytes;
    HANDLE hHdrs;
    LPCARDHEADER Cards;
    CARD Card;
    OFSTRUCT OF;

    if (!fValidate)
        return TRUE;

    /* open file */
    if ((fh = OpenFile(szFile, &OF, 
                       OF_CANCEL | OF_READ | OF_SHARE_DENY_WRITE)) < 0)
    {
        return FALSE;
    }

    /* try to read the cardfile */

    /* read the cardfile signature */
    Signature[3] = 0;
    myread(fh, Signature, 3);
    if (lstrcmp(Signature, szMGC) == 0)
        fOld = TRUE;
    else if (lstrcmp(Signature, szRRG) == 0)
    {
        fOld = FALSE;
        /* read in the max id */
        myread(fh, &nObjectMax, sizeof(DWORD));
        if (nObjectMax != idObjectMax)
            return ValidCardfileFailed(fh);
    }
    else
        return ValidCardfileFailed(fh);

    /* read the number of cards in the file */
    myread(fh, &nCards, sizeof(int));
    /* limit data to < 64k. */
    cBytes = mylmul(nCards,sizeof(CARDHEADER));
    if (nCards != cCards || cBytes >= 0x0000FFFF ||
        !(hHdrs = GlobalAlloc(GHND, (DWORD)((WORD)sizeof(CARDHEADER)*(WORD)nCards))) ||
        !(Cards = (LPCARDHEADER) GlobalLock(hHdrs)))
        return ValidCardfileFailed(fh);

    /* read all the header into memory */
    for (i = 0; i < nCards; i++)
    {
        mylread(fh, &Cards[i], sizeof(CARDHEADER));
    }

    /* read each card data */
    for (i = 0; i < nCards; i++)
    {
        /* seek to card data */
        if (_llseek(fh, Cards[i].lfData, 0) == -1 ||
            !PicRead(&Card, fh, !fOLE || fOld) ||
            TextRead(fh, szText) < 0)
        {
            GlobalUnlock(hHdrs);
            GlobalFree(hHdrs);
            return ValidCardfileFailed(fh);
        }
        if (Card.lpObject)
            PicDelete(&Card);
    }

    GlobalUnlock(hHdrs);
    GlobalFree(hHdrs);
    CloseHandle(fh);
    return TRUE;
}

/* 
 * FileHasTextOnly(hCards, fhMain, fhTemp);
 * returns TRUE if the file has text only.
 * In this case, it is preferable to save in Win 3.0 format since it can be 
 * read by both 3.0 and 3.1 cardfile.exe.
 */
NOEXPORT BOOL FileHasTextOnly( HANDLE hCards,
                               int fhMain, 
                               int fhTemp)
{
    LPCARDHEADER Cards;
    BOOL fTextOnly = TRUE;
    int i;
    int fh;
    int bmSize;

    /* lock down the card headers */
    Cards = (LPCARDHEADER) GlobalLock(hCards);
    for (i = 0; i < cCards && fTextOnly; i++)
    {
        /* pick up card data from src or temp file */
        if (Cards[i].flags & FTMPFILE)
        {
            fh = fhTemp;
        }
        else
        {
            fh = fhMain;
        }

        /* Does the card contain a picture? */
        /* Read the Boolean value */
        if ((_llseek(fh, Cards[i].lfData, 0) == -1) || (myread(fh, &bmSize, sizeof(int)) != sizeof(int)) )
        {
            fTextOnly = FALSE;        /* read error, assume there is a picture */
        }
        if (bmSize) /* Read OK. if nonzero, there exists a picture */
        {
            fTextOnly = FALSE;        /* there is a picture */
        }
    }
    GlobalUnlock(hCards);
    return fTextOnly;
}
