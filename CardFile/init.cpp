#include "cardfile.h"
#include <shellapi.h>

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2003 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/

/* OLE definitions */
OLECLIENTVTBL clientTbl;
OLESTREAMVTBL streamTbl;
UINT vcfLink = 0;
UINT vcfOwnerLink = 0;
UINT vcfNative = 0;
OLESTATUS oleloadstat;
LHCLIENTDOC lhcdoc = NULL;

/* Class name for cardfile - not translatable. */
char szCardfileClass[] = "Cardfile";
char szCardClass[] = "Card";

/* Name of cardfile that appears in title bar - translatable. */
char szCardfile[40];
char szMarginError[160];
char szFileExtension[5];

/* Headings for sections of win.ini */
char szWindows[] = "Windows";
char szDevice[] = "Device";
char szCardfileSect[] = "Cardfile";

/* Things for Page Setup */
char chPageText[6][PT_LEN];

unsigned wMerge;

/* variables for the new File Open,File SaveAs and Find Text dialogs */
#define FILTERMAX   100
#define CAPTIONMAX  30

OPENFILENAME OFN;
FINDREPLACE FR;
PRINTDLG PD;
char szLastDir    [PATHMAX];
HANDLE hFind = NULL;
char szServerFilter[FILTERMAX * 10]; /* default filter spec. for servers  */
char szFilterSpec [FILTERMAX * 4];    /* default filter spec. for above    */
char szCustFilterSpec[FILTERMAX];     /* buffer for custom filters created */
int wFRMsg;                  /* message used in communicating       */
                      /* with Find/Replace dialog       */
int wHlpMsg;                  /* message used to invoke Help       */
char szOpenCaption [CAPTIONMAX];      /* File open dialog caption text       */
char szSaveCaption [CAPTIONMAX];      /* File Save as dialog caption text  */
char szMergeCaption[CAPTIONMAX];      /* File Merge dialog caption text    */
char szLinkCaption [CAPTIONMAX];      /* Link Repair dialog caption text   */

HACCEL hAccel;

NOEXPORT BOOL NEAR ProcessShellOptions(
    LPSTR lpLine);

/*
 * do this for every instance
 */
BOOL InitInstance( HINSTANCE hInstance,
                   LPSTR lpszCommandLine,
                   int cmdShow)
{
    int i;
    LPCARDHEADER Cards;
    HWND hwnd = NULL;
    LPSTR lpchTmp;
    PSTR pchTmp;
    char buf[3];
    int  cbName;
    HMENU hMenu;
    BOOL bExt;
    char *pszFilterSpec = szFilterSpec;

    if (fOLE && !OleInit(hInstance))
        goto InitError;

    LoadString(hInstance, EINSMEMORY, NotEnoughMem, sizeof(NotEnoughMem));
    LoadString(hInstance, IHELPFILE, szHelpFile, 30);
    LoadString(hInstance, IUNTITLED, szUntitled, 30);
    LoadString(hInstance, IWARNING, szWarning, 30);
    LoadString(hInstance, INOTE, szNote, 30);
    LoadString(hInstance, ISTRINGINSERT, buf, 3);
    LoadString(hInstance, IFILEEXTENSION, szFileExtension, 5);
    LoadString(hInstance, ICARDFILE, szCardfile, 40);
    LoadString(hInstance, IMARGINERR, szMarginError, 160);

    LoadString(hInstance, IDS_OPENDLGTITLE, szOpenCaption, CAPTIONMAX);
    LoadString(hInstance, IDS_SAVEDLGTITLE, szSaveCaption, CAPTIONMAX);
    LoadString(hInstance, IDS_MERGEDLGTITLE, szMergeCaption, CAPTIONMAX);
    LoadString(hInstance, IDS_LINKTITLE, szLinkCaption, CAPTIONMAX);
    LoadString(hInstance, IDS_OBJNAME, szObjFormat, CAPTIONMAX);

    /* Go grab the default page Setup parameters */
    for (i=0; i<6; i++)
        LoadString(hInstance, IHEADER+i, chPageText[i], 20);

    if (!LoadString(hInstance, EINSMEMORY, NotEnoughMem, 160))
        goto InitError;

    wMerge = *(unsigned *)buf;

    //lpDlgProc = MakeProcInstance( DlgProc, hInstance );   DONE
    //lpfnDial = MakeProcInstance( fnDial, hInstance );     DONE
    //lpfnAbortProc = MakeProcInstance( fnAbortProc, hInstance);
    //lpfnPageDlgProc = MakeProcInstance( PageSetupDlgProc, hInstance); DONE
    //lpfnAbortDlgProc = MakeProcInstance( fnAbortDlgProc, hInstance);
    //lpfnLinksDlg = MakeProcInstance(fnLinksDlg, hInstance);            DONE
    //lpfnInvalidLink = MakeProcInstance(fnInvalidLink, hInstance);

    /* unlikely that last one will work but others won't */
    //if (!lpfnAbortDlgProc)
    //    goto InitError;

    fValidate = GetProfileInt(szCardfileSect, szValidateFileWrite, 1);
    hCards = GlobalAlloc(GHND, (long)sizeof(CARDHEADER)); /* alloc first card */
    if (!hCards)
        goto InitError;

    iTopScreenCard = 0;

    /* make a single blank card */
    CurIFile[0] = 0;        /* file is untitled */
    cCards = 1;
    CurCardHead.line[0] = 0;
    CurCard.lpObject = NULL;
    CurCardHead.flags = FNEW;
    Cards = (LPCARDHEADER)GlobalLock(hCards);
    Cards[0] = CurCardHead;
    GlobalUnlock(hCards);

    /* this is the main window */

    hwnd = CreateWindowEx(WS_EX_WINDOWEDGE, szCardfileClass, NULL,
                          WS_OVERLAPPEDWINDOW, /* | WS_CLIPCHILDREN, */
                          CW_USEDEFAULT, 0,
#ifdef JAPAN
                          /* eight must be less that 400 */
                          CardWidth*4/3, min(GetSystemMetrics(SM_CYSCREEN)-GetSystemMetrics(SM_CYBORDER)*2,CardHeight*7/4),
#else
                          CardWidth*4/3, CardHeight*7/4,
#endif
                          NULL, NULL, hInstance, NULL);

    if (!hwnd)
    {
        InitError:
        MessageBox(hwnd, NotEnoughMem, NULL, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
        return FALSE;
    }

    DragAcceptFiles(hwnd, TRUE);

    hMenu = GetMenu(hwnd);
    CheckMenuItem(hMenu,CCARDFILE,MF_CHECKED);

    MakeTempFile();

    /* init fields of the PRINTDLG structure (not used yet) */
    PD.lStructSize    = sizeof(PRINTDLG);
    PD.hwndOwner      = hwnd;
    PD.hDevMode       = NULL;
    PD.hDevNames      = NULL;
    PD.hDC            = NULL;
    PD.nCopies        = 1;


    ShowWindow(hwnd, cmdShow);

    if (ProcessShellOptions(lpszCommandLine))
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0L);
        return TRUE;
    }

/* for "foo.crd", you get 07foo.crd00 with
 * lpszCommandLine pointing at foo.  use length given in the
 * previous byte, rather than searching for a null terminator, because
 * real msdos does not null terminate (it terminates with a CR).
 * above worked except for chars above 128 which it thought were < space.
 * 28-Oct-1987. davidhab.
 */
    for (cbName = (int)(*(lpszCommandLine - 1)), bExt = FALSE,
         i = 0, lpchTmp = lpszCommandLine, pchTmp = CurIFile; i < cbName; i++)
    {
        switch (*pchTmp++ = *lpchTmp++)
        {
        case '\\':
        case '/':
            bExt = FALSE;
            break;
        case '.':
            bExt = TRUE;
            break;
        default:
            break;
        }
#ifdef DBCS
        // pchTmp may be on a DBCS lead byte, so we
        // check it and make an extra move if 
        // needed. LiZ -- 5/14/93
        if ( IsDBCSLeadByte(*(pchTmp-1)) )
        {
            *pchTmp++ = *lpchTmp++;
            i++;
        }
#endif
    }

    *pchTmp = 0;

    if (*CurIFile)
    {
        OFSTRUCT of;

        /* Get a fully qualified pathname */
        of.cBytes = sizeof(OFSTRUCT);
        if (!bExt)
        {
            lstrcat(CurIFile, ".");
            lstrcat(CurIFile, szFileExtension);
        }
        if ((OpenFile(CurIFile, &of, OF_EXIST) < 0) && !bExt)
        {
            *pchTmp = 0;
            OpenFile(CurIFile, &of, OF_PARSE);
        }
        OemToAnsi(of.szPathName, CurIFile);
    }

    /* construct default filter string in the required format for
     * the new FileOpen and FileSaveAs dialogs.  Pretty brutish...
     */

    LoadString(hInstance, IDS_FILTERSPEC, pszFilterSpec, FILTERMAX);
    pszFilterSpec += lstrlen (szFilterSpec) + 1;
    *pszFilterSpec++ = '*';
    *pszFilterSpec++ = '.';
    lstrcat(pszFilterSpec, szFileExtension);
    pszFilterSpec += lstrlen(pszFilterSpec) + 1;

    LoadString(hInstance, IDS_FILTERSPEC2, pszFilterSpec, FILTERMAX);
    pszFilterSpec += lstrlen (pszFilterSpec) + 1;
    *pszFilterSpec++ = '*';
    *pszFilterSpec++ = '.';
    lstrcat(pszFilterSpec, szFileExtension);
    pszFilterSpec += lstrlen(pszFilterSpec) + 1;

    LoadString(hInstance, IDS_FILTERSPEC3, pszFilterSpec, FILTERMAX);
    pszFilterSpec += lstrlen(pszFilterSpec) + 1;
    lstrcat(pszFilterSpec, "*.*");
    pszFilterSpec += lstrlen(pszFilterSpec) + 1;
    *pszFilterSpec++ = '\0';
    *szCustFilterSpec = '\0';

    /* init. some fields of the OPENFILENAME struct used by fileopen and
     * filesaveas
     */
    OFN.lStructSize       = sizeof(OPENFILENAME);
    OFN.hwndOwner      = hwnd;
    OFN.nMaxCustFilter      = FILTERMAX;
    OFN.nFilterIndex      = 1;
    OFN.nMaxFile      = PATHMAX;
    OFN.lpfnHook          = NULL;
    OFN.hInstance         = hInstance;
    OFN.lpstrFileTitle    = NULL;
    *szLastDir            = 0;

    /* init.fields of the FINDREPLACE struct used by FindText() */
    FR.lStructSize        = sizeof(FINDREPLACE);
    FR.hwndOwner      = hwnd;
    FR.hInstance      = hInstance;
    FR.lpTemplateName = MAKEINTRESOURCE(DTFINDDLG);
    FR.Flags          = FR_ENABLETEMPLATE | FR_HIDEWHOLEWORD | FR_DOWN;
    FR.lpstrReplaceWith   = NULL;  /* not used by FindText() */
    FR.wReplaceWithLen    = 0;     /* not used by FindText() */
    FR.lpfnHook           = NULL;
    FR.wFindWhatLen       = LINELENGTH;
    if (!(hFind = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, PATHMAX)))
        return FALSE;


    /* determine the message number to be used for communication with
     * Find dialog
     */
    if (!(wFRMsg = RegisterWindowMessage ((LPSTR)FINDMSGSTRING)))
        return FALSE;
    if (!(wHlpMsg = RegisterWindowMessage ((LPSTR)HELPMSGSTRING)))
        return FALSE;

    if (*CurIFile)
    {
        if (fOLE && S_OK != OleRegisterClientDoc(szCardfile, CurIFile, 0L, &lhcdoc))
            ErrorMessage(W_FAILED_TO_NOTIFY);

        if (!DoOpen(CurIFile))
            goto FallThrough;

        SetCurCard(iFirstCard);     /* Don't discard too soon */
        SetNumOfCards();
    }
    else
    {
        FallThrough:
        CurIFile[0] = 0;    /* failed, make untitled */
        if (fOLE && S_OK != OleRegisterClientDoc(szCardfile, szUntitled, 0L, &lhcdoc))
            ErrorMessage(W_FAILED_TO_NOTIFY);
    }

    return(TRUE);
}

/*
 * Processes command line params specified by the shell
 *
 * If command line for cardfile has 
 *      "/PC <filename>" - print all the cards
 *      "/PL <filename>" - print the list
 */
NOEXPORT BOOL NEAR ProcessShellOptions(
    LPSTR lpLine)
{
    char PrintCode;
    char szFile[PATHMAX];
    OFSTRUCT of;

    AnsiUpper(lpLine);

    // skip spaces
    while (*lpLine == ' ' || *lpLine == '\t')
        lpLine++;

    if (lpLine[0] != '/' || lpLine[1] != 'P')
        return FALSE;
    
    lpLine += 2;
    if(*lpLine == 'C' ||  *lpLine == 'L') /* Code specifies list or cards. */
    	PrintCode = *lpLine++;
    else if(*lpLine == ' ') 		  /* Deafault print card files. /P */
    	PrintCode = 'C';
    else				  /* Unrecognizable code.          */
    	return FALSE;

    /* skip spaces */
    while (*lpLine == ' ' || *lpLine == '\t')
        lpLine++;

    if (!*lpLine)
        return FALSE;

    UpdateWindow(PD.hwndOwner);

    /* Get a fully qualified pathname */
    of.cBytes = sizeof(OFSTRUCT);
    OpenFile(lpLine, &of, OF_PARSE);
    OemToAnsi(of.szPathName, szFile);

    if (!OpenNewFile(szFile)) 
        return TRUE;

    if (PrintCode == 'C')    /* print cards */
        PrintCards(cCards);
    else
        PrintList();
    
    return TRUE;
}

/*
 * do one time global initilization for Cardfile
 */

BOOL IndexInit( void )
{
    WNDCLASS    rClass;
    HICON    hIndexIcon;
    TEXTMETRIC    Metrics;    /* structure filled with font info */
    HDC        hIC;
    HGDIOBJ    hOldFont;

    /* get a memory dc for cheap dc stuff */

    if (!(hIC = CreateIC("DISPLAY", NULL, NULL, NULL)))
        return FALSE;

    hOldFont = SelectObject(hIC, hFont);
    /* Setup the fonts */
    GetTextMetrics(hIC, &Metrics);  /* find out what kind of font it really is */
    SelectObject(hIC, hOldFont);

    DeleteDC(hIC);

    CharFixHeight = Metrics.tmHeight+Metrics.tmExternalLeading;    /* the height */
    ExtLeading      = Metrics.tmExternalLeading;
    CharFixWidth  = Metrics.tmAveCharWidth;
    ySpacing      = CharFixHeight+1;

    cxHScrollBar  = GetSystemMetrics(SM_CXHSCROLL);
    cyHScrollBar  = GetSystemMetrics(SM_CYMENU);

    CardWidth  = (LINELENGTH * CharFixWidth) + 3;
    CardHeight = (CARDLINES*CharFixHeight) + CharFixHeight + 1 + 2 + 2;

    EditWidth  = (LINELENGTH * CharFixWidth) + 1;
    EditHeight = (CARDLINES*CharFixHeight);

    /* get the resource file info, such as icons, and IT tables */
    hArrowCurs    = LoadCursor(NULL, IDC_ARROW);
    hWaitCurs    = LoadCursor(NULL, IDC_WAIT);
    hIndexIcon  = LoadIcon(hIndexInstance,(LPSTR)INDEXICON);
    hAccel = LoadAccelerators(hIndexInstance, (LPSTR)MAINACC);
    if (!hArrowCurs || !hWaitCurs || !hIndexIcon || !hAccel)
        return(FALSE);

    /* make rClass */
    rClass.lpszClassName = (LPSTR)szCardfileClass;
    rClass.hCursor       = hArrowCurs;    /* normal cursor is arrow */
    rClass.hIcon         = hIndexIcon;
    rClass.hbrBackground = (HBRUSH) COLOR_3DSHADOW;    /* this changes */
    rClass.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_BYTEALIGNCLIENT;
    rClass.lpfnWndProc   = IndexWndProc;
    rClass.hInstance     = hIndexInstance;
    rClass.lpszMenuName  = (LPSTR)MTINDEX;
    rClass.cbClsExtra    = 0;
    rClass.cbWndExtra    = 0;

    if (!RegisterClass(&rClass))
        return FALSE;

    /* make CardClass */
    rClass.lpszClassName = (LPSTR)szCardClass;
    rClass.hCursor       = hArrowCurs;    /* normal cursor is arrow */
    rClass.hIcon         = NULL;
    rClass.hbrBackground = (HBRUSH) COLOR_3DSHADOW;
    rClass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_BYTEALIGNCLIENT;
    rClass.lpfnWndProc   = CardWndProc;
    rClass.hInstance     = hIndexInstance;
    rClass.lpszMenuName  = NULL;

    if (!RegisterClass(&rClass))
        return FALSE;

    return TRUE;
}

BOOL OleInit(
    HANDLE hInstance) 
{
    HANDLE      hobjStream = NULL;
    HANDLE      hobjClient = NULL;

    vcfLink      = RegisterClipboardFormat("ObjectLink");
    vcfNative    = RegisterClipboardFormat("Native");
    vcfOwnerLink = RegisterClipboardFormat("OwnerLink");
    lpclient     = NULL;
    lpStream     = NULL;

    if (!(hobjClient = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,sizeof(OLECLIENT))))
            goto Error;

    if (!(lpclient = (LPOLECLIENT)(GlobalLock(hobjClient))))
        goto Error;

    lpclient->lpvtbl     = (LPOLECLIENTVTBL)&clientTbl;

    if (!(hobjStream = GlobalAlloc (GMEM_MOVEABLE, sizeof(CARDSTREAM))))
        goto Error;

    lpclient->lpvtbl->CallBack = CallBack;

    if (!(lpStream = (LPCARDSTREAM)(GlobalLock(hobjStream))))
        goto Error;

    lpStream->lpstbl = (LPOLESTREAMVTBL)&streamTbl;
    //pfOldRead        = (DWORD (FAR PASCAL *)(LPOLESTREAM, LPSTR, DWORD))
    //                    MakeProcInstance((FARPROC)ReadOldStream, hInstance);
    //pfNewRead =
    //streamTbl.Get    = (DWORD (FAR PASCAL *)(LPOLESTREAM, LPSTR, DWORD))
    //                    MakeProcInstance((FARPROC)ReadStream, hInstance);
    //streamTbl.Put    = (DWORD (FAR PASCAL *)(LPOLESTREAM, LPSTR, DWORD))
    //                    MakeProcInstance((FARPROC)WriteStream, hInstance);
    //
    pfOldRead        = ReadOldStream;

    pfNewRead =
    streamTbl.Get    = ReadStream;

    streamTbl.Put    = WriteStream;

    lpStream->hobjStream = hobjStream;
    return TRUE;

Error:
    if (lpStream)
        GlobalUnlock(hobjStream);
    if (hobjStream)
        GlobalFree(hobjStream);
    if (lpclient)
        GlobalUnlock(hobjClient);
    if (hobjClient)
        GlobalFree(hobjClient);

    return FALSE;
}
