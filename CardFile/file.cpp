#include "cardfile.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

int fNoTempFile = FALSE;
char TempFile[PATHMAX];
OFSTRUCT ofTempFile;

char SavedIndexLine[LINELENGTH+1];


/* Read text data for a card. */
int TextRead( HANDLE fh,
              char *szBuf)
{
    int tSize;

    *szBuf = 0;
    /* get text size */
    if (myread(fh, &tSize, sizeof(int)) < sizeof(int))
        return -1;
    /* truncate it if too long */
    if (tSize >= CARDTEXTSIZE)
        tSize = CARDTEXTSIZE-1;
    /* read the text */
    if (mylread(fh, szBuf, tSize) < tSize)
        return -1;
    szBuf[tSize] = 0;

    return tSize;
}

/* append cardfile extension to the given filename */
void AppendExtension( char *pName,
                      char *pBuf)
{
    char *p;
    char ch;

    lstrcpy(pBuf, pName);
    p = pBuf + lstrlen(pBuf);
    while ((ch = *p) != '.' && ch != '\\' && ch != ':' && p > pBuf)
        p = (PSTR)(DWORD)AnsiPrev(pBuf, p);
    if (*p != '.')
        wsprintf(pBuf, "%s.%s", (LPSTR)pName, (LPSTR)szFileExtension);
    AnsiUpper(pBuf);
}

/*
 * write CardHead, Card object and Text
 */
int WriteCurCard( PCARDHEADER pCardHead,
                  PCARD pCard,
                  char *pText)
{
    HANDLE fh;
    DWORD lEnd;
    WORD tSize;
    int Result;

    // replace OpenFile with CreateFile
    //if ((fh = CreateFile(TempFile, &ofTempFile, 
    //                   OF_SHARE_EXCLUSIVE | OF_CANCEL | OF_REOPEN | OF_READWRITE)) == HFILE_ERROR )

    fh = CreateFile( TempFile,                        // file name
                          GENERIC_READ | GENERIC_WRITE,    // access mode
                          0,                               // share mode
                          NULL,                            // SD
                          CREATE_NEW,                      // how to create
                          FILE_ATTRIBUTE_NORMAL,           // file attributes
                          NULL);
    if (fh == INVALID_HANDLE_VALUE )
    {
        return IndexOkError(EOPENTEMPSAVE);
    }

    // TODO: replace _llseek with SetFilePointer
    //lEnd = _llseek(fh, 0L, 2);  /* seek to the end of the file */
    lEnd = SetFilePointer( fh, 0,NULL ,FILE_END);

    Hourglass(TRUE);

    tSize = lstrlen(pText);
    if (tSize >= CARDTEXTSIZE)
        tSize = CARDTEXTSIZE-1;

    /* If an embedded object is open for editing, try to update */
    Result = UpdateEmbObject(&CurCard, MB_YESNO);
    if (Result != IDYES && !fInsertComplete)
    {
        PicDelete(pCard);
        fInsertComplete = TRUE;
    }

    if (!PicWrite(pCard, fh, FALSE) ||                      /* write object, */
        mywrite(fh, &tSize, sizeof(int)) < sizeof(int) ||   /* text size,    */
        mylwrite(fh, pText, tSize) < tSize)                 /* text.         */
    {
        CloseHandle(fh);
        Hourglass(FALSE);
        return IndexOkError(EDISKFULLSAVE);
    }

    // replace _lclose with CloseHandle
    CloseHandle(fh);
    pCardHead->flags |= FTMPFILE;
    pCardHead->lfData = lEnd;

    /* Make a copy of the current index line for restore purposes */
    lstrcpy(SavedIndexLine, pCardHead->line);

    Hourglass(FALSE);
    return(TRUE);
}

int ReadCurCardData( PCARDHEADER pCardHead,
                     PCARD pCard,
                     char *pText)
{
    HANDLE fh;
    BOOL fOld;

    /* !!!bozo check for deleteable objects here? */

    if (pCardHead->flags & FNEW)
        return TRUE;

    if (pCardHead->flags & FTMPFILE)
    {
        fh = CreateFile( TempFile,                        // file name
                              GENERIC_READ | GENERIC_WRITE,    // access mode
                              0,                               // share mode
                              NULL,                            // SD
                              CREATE_NEW,                      // how to create
                              FILE_ATTRIBUTE_NORMAL,           // file attributes
                              NULL );
        // replace OpenFile with CreateFile
        //fh = OpenFile(TempFile, &ofTempFile, 
        //              OF_SHARE_EXCLUSIVE | OF_REOPEN | OF_READWRITE | OF_CANCEL);
        //

        fOld = FALSE;
    } 
    else
    {
        fh = fhMain;
        fOld = fOldCardfile;
    }

    if (fh == INVALID_HANDLE_VALUE)
    {
        return BuildAndDisplayMsg(E_CANT_REOPEN_FILE, 
                                  (pCardHead->flags & FTMPFILE)? TempFile: CurIFile);
    }

    Hourglass(TRUE);
    // TODO: replace _llseek with SetFilePointer
    //if (_llseek(fh, pCardHead->lfData, 0) == -1 ||
    if (SetFilePointer(fh, pCardHead->lfData,NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER ||
        !PicRead(pCard, fh, !fOLE || fOld) ||   /* read object  */
        TextRead(fh, pText) < 0)                /* and the text */
    {
        if (pCardHead->flags & FTMPFILE)
            CloseHandle(fh);
        IndexOkError(E_FAILED_TO_READ_CARD);
    }

    Hourglass(FALSE);

    if (pCardHead->flags & FTMPFILE)
    {
        CloseHandle(fh);
    }

    /* Make a copy of the Index line for restoring latter */
    lstrcpy(SavedIndexLine, pCardHead->line);

    return TRUE;
}


/* return ptr to the first occurrence of the given char in the string */
/* return ptr to filename in the given path */
LPSTR FileFromPath( LPSTR lpStr)
{
    LPSTR lp;

    lp = StrRChr(lpStr, NULL, '\\');
    if (lp == NULL)
        return lpStr;
    else
        return(lp+1);
}


/* create space in the array of hdrs for n more cards */
BOOL ExpandHdrs( int n)
{
    DWORD cBytes;

    cBytes = mylmul((cCards+n),sizeof(CARDHEADER));
    /* if data > 64K or reallocation fails, return failure */
    if (cBytes >= 0x0000FFFF ||
        !GlobalReAlloc(hCards, cBytes, GMEM_MOVEABLE))
    {
        return IndexOkError(EINSMEMORY);
    }

    return TRUE;
}

