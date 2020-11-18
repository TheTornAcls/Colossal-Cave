/*
 * cardfile.h main include file for CARDFILE
 */
#include "windows.h"
#include "indexrc.h"
#include <commdlg.h>                            /* Standard dialogs */
#include "ecda.h"                /* OLE */
#include "shlwapi.h"

#define	KEYNAMESIZE	300
#define OBJSTRINGSMAX 64

#ifdef DEBUG
#define NOEXPORT 
#else
#define NOEXPORT static
#endif
#define     ABOUT       100
#define     NEW         101
#define     OPEN        102
#define     SAVE        103
#define     SAVEAS      104
#define     PRINT       105
#define     PRINTALL    106
#define     MERGE       107
#define     EXIT        108
#define     PAGESETUP   109
#define     PRINTSETUP  110

#define     CCARDFILE   141
#define     PHONEBOOK   142
#define     VIEW        143

#define     UNDO        118
#define     HEADER      111
#define     RESTORE     112
#define     CUT         113
#define     COPY        114
#define     PASTE       115
#define     I_TEXT      116
#define     I_OBJECT    117
#define     ADD         121
#define     CDELETE     122
#define     CDUPLICATE  123
#define     DIAL        124
#define     GOTO        131
#define     FIND        132
#define     FINDNEXT    133
#define     IDM_PASTESPECIAL    134
#define     IDM_INSERTOBJECT    135


/* Use standard help numbers */
#define     ID_INDEX    301             //0xFFFF
#define     ID_SEARCH   302             //0x0021
#define     ID_USEHELP  303             //0XFFFC


#define     EDITWINDOW      200
#define     LEFTWINDOW      201
#define     RIGHTWINDOW     202
#define     SCROLLWINDOW    203
#define     LISTWINDOW      204
#define     CARDWINDOW      205

/* -------------------------------------------------------------------- */
/* Added conditional compilation for long filename support under OS/2   */
/* t-carlh - August, 1990                                               */
/* -------------------------------------------------------------------- */
#define PATHMAX 128     /* Maximum filename length */

#define PT_LEN  50              /* max length of page setup strings */

extern char szText[];           /* buffer for reading in card text, etc */
extern char DefaultNullStr[];

extern BOOL bAskForUpdate;

extern int wFRMsg;              /* Registered "Find/Replace" message */

extern char *szOpenMergeText;

extern HWND hListWnd;
extern HWND hCardWnd;
extern HWND hIndexWnd;
extern HWND hEditWnd;
extern HWND hLeftWnd;
extern HWND hRightWnd;
extern HWND hScrollWnd;
extern HWND hEditWnd;
extern HWND hDlgFind;
extern BOOL fInSaveAsDlg;
extern BOOL fValidate; 
extern char szValidateFileWrite[];

extern HACCEL hAccel;
extern HANDLE hEditCurs;
extern HINSTANCE hIndexInstance;
extern HGDIOBJ hFont;
extern HDC hMemoryDC;

extern int EditWidth;
extern int EditHeight;

extern int xCardWnd;        /* size of the Card window */
extern int yCardWnd;    
extern int cxHScrollBar;
extern int cyHScrollBar;

extern int CardPhone;
extern int fCanPrint;
extern BOOL bPrinterSetupDone;
extern int fNeedToUpdateObject;
extern int fSettingText;

extern OPENFILENAME OFN;
extern FINDREPLACE FR;
extern PRINTDLG PD;

extern char szFileName[];
extern char TempFile[];
extern char szOpenCaption[];       /* File open dialog caption text    */
extern char szSaveCaption[];       /* File Save as dialog caption text    */
extern char szMergeCaption[];      /* File Merge dialog caption text    */

extern char chPageText[6][PT_LEN];
extern char SavedIndexLine[];
extern int DBcmd;
extern int ySpacing;


extern char szPrinter[];
extern char szMarginError[];
extern char szDec[5];
extern int iMeasure;

extern char CurIFile[];
extern char szSearch[];
extern BOOL fCase, fReverse, fRepeatSearch;

//extern FARPROC lpfnAbortProc;
//extern DLGPROC lpfnAbortDlgProc;
//extern DLGPROC lpfnPageDlgProc;
extern FARPROC lpPrinterSetDlg;
//extern DLGPROC lpDlgProc;
extern FARPROC lpfnOpen;
extern FARPROC lpfnSave;
//extern DLGPROC lpfnDial;
extern WNDPROC lpEditWndProc;
//extern DLGPROC lpfnLinksDlg;         /* OLE */
//extern FARPROC lpfnInvalidLink;        /* OLE */

extern char szCardView[];
extern char szListView[];

extern OFSTRUCT ofMainFile;
extern HANDLE fhMain; /* file handle for source file */
extern BOOL fOldCardfile;        /* Are we in the compatibility zone? */
extern int fReadOnly;
extern char szValidateFileWrite[];   
extern BOOL fValidate;  /* TRUE if validating on save */
extern char TempFile[];
extern OFSTRUCT ofTempFile;
extern int fNoTempFile;
extern char szRRG[];       /* new cardfile signature.(OLE) */
extern char szMGC[];       /* old cardfile signature. */
extern char SavedIndexLine[];


#define LINELENGTH  40
#define CARDLINES   11
#define CARDTEXTSIZE    ((LINELENGTH+1) * CARDLINES)
#define CARDCY      12

typedef struct tagTIME
{
    int chSep;              /* Separator character for date string */
    char sz1159[6];         /* string for AM */
    char sz2359[6];         /* string for PM */
    BOOL f24Hr;             /* TRUE if military time */
    BOOL fLeadZeroHour;     /* lead zero for hour */
} TIME;

/* DATE.Flags and DATEFORMAT.Flags values to indicate 
 * Century, lead zeros for day and month */
#define LEADZEROMONTH           0x0001        /* 2 vs. 02 */
#define LEADZERODAY             0x0002        /* 2 vs. 02 */
#define CENTURY                 0x0004        /* 1990 vs 90 */

/* max size of short date format string "dd/MM/yyyy" */
#define MAX_FORMAT              11

typedef struct tagDATE {
    /* order of month, day and year. 0 => MDY, 1=> DMY, 2=> YMD */
    int Order;     
    int chSep;      /* Separator character for date string */
    char szShortFormat[MAX_FORMAT];
    int Flags;      /* Century - 1990 vs. 90, lead zeros for day and month */
} CARDDATE;
    
typedef struct
{
    char     reserved[6];               /* bytes available for future use */
    unsigned long    lfData;            /* file offset of data */
    unsigned char    flags;             /* flags */
    char     line[LINELENGTH+1];        /* 40 character lines plus null */
} CARDHEADER;

typedef CARDHEADER *PCARDHEADER;
typedef CARDHEADER far *LPCARDHEADER;

typedef struct 
{
    /*
     * For OLE, the BITMAP is just a special frozen object.
     *
     * If we're in compatibility mode (lpObject is an HBITMAP) the 
     * selector of lpObject will be NULL.
     */
    DWORD       idObject;        /* What is the object name? */
    OBJECTTYPE  otObject;        /* What kind of object is this? */
    RECT        rcObject;        /* Where should the object be drawn? */
    DWORD       lpObject;        /* Handle to the object */
    HANDLE      hText;
} CARD, *PCARD;

extern DWORD idObjectMax;

extern int cCards;        /* the current count of cards */
extern HANDLE hCards;        /* the handle to the buffer */

extern int CharFixWidth;    /* the character width */
extern int CharFixHeight;   /* the character height */
extern int ExtLeading;        /* the external leading */

extern int CardWidth;
extern int CardHeight;
extern int fColor;

extern HBRUSH hbrCard;
extern HBRUSH hbrBorder;
extern HCURSOR hArrowCurs;
extern HCURSOR hWaitCurs;

extern CARDHEADER CurCardHead;
extern CARD CurCard;

#define FNEW        0x01
#define FDIRTY      0x02
#define FTMPFILE    0x04

/* Return values from OleError: No problem, error with/without warning */
#define FOLEERROR_OK       0x00
#define FOLEERROR_GIVEN    0x01
#define FOLEERROR_NOTGIVEN 0x02

extern RECT dragRect;

extern int iTopScreenCard;
extern int iCurCard;
extern int iFirstCard;

#define LEFTMARGIN      8
#define TOPMARGIN       8
#define BOTTOMMARGIN    8

extern int cScreenHeads;
extern int cScreenCards;
extern int cFSHeads;        /* count of fully visible headers */

#define SBINDEX SB_HORZ

extern int ySpacing;
extern int xFirstCard;
extern int yFirstCard;

extern int EditMode;

extern char *pcComNum;
extern char *pcTonePulse;
extern char *pcSpeedNum;

extern unsigned wMerge;        /* two characters for string insertion */

extern BOOL fFileDirty;        /* does disk match current set of cards? */
extern BOOL fFullSize;

extern BOOL fInsertComplete;    /* If insertObject in progress, it is FALSE */
                                /* Reset to TRUE when InsertObjectis complete */

#define INDEXICON   1
#define MAINACC     1
#define MTINDEX     1

/* prompts */
#define IDELCURCARD     0
#define ICREATEFILE     1
#define IOKTOSAVE       2
#define ICARDS          3
#define ICARD           4
#define IONECARD        5
#define IUNTITLED       6
#define ICARDDATA       7
#define IWARNING        8
#define INOTE           9
#define IPICKUPPHONE    10
#define ICARDFILE       11
#define IMARGINERR      12
#define IMERGE          13
#define IOPEN           14
#define ICARDVIEW       15
#define ILISTVIEW       16
#define IHELPFILE       18


/* errors */
#define ECANTDIAL       19
#define ECANTPRINTPICT  20
#define EINSMEMORY      21
#define EINVALIDFILE    23
#define ECLIPEMPTYTEXT  24
#define ENOTEXTSEL      25
#define EDISKFULLFILE   26
#define ECANTMAKETEMP   27
#define EINSMEMSAVE     28
#define EDISKFULLSAVE   29
#define EOPENTEMPSAVE   30
#define ECANTREADPICT   31
#define EINSMEMRUN      32
#define ENOTVALIDFILE   33
#define ECANTFIND       34
#define EINSMEMREAD     35
#define ECANTPRINT      36
#define ECLIPEMPTYPICT  37
#define ENOMODEM        38
#define ENOPICTURES     39
#define ECANTMAKEFILE   40
#define EMEMPRINT       41
#define EDISKPRINT      42
#define ISTRINGINSERT   43
#define IFILEEXTENSION  44

/**** WARNING!  DO NOT CHANGE THE ORDER OF THESE! ***/
#define IHEADER         45
#define IFOOTER         46
#define ILEFT           47
#define IRIGHT          48
#define ITOP            49
#define IBOT            50
/*** END OF WARNING ***/

#define IDS_LETTERS      51
/* A whole slew of E_ errors defined in ecda.h, starting at 0x100 */
/* A few W_ errors defined in ecda.h, starting at 0x200 */
#define E_CANT_REOPEN_FILE  53
#define E_FILEUPDATEFAILED  54
#define E_FILEWRITEFAILED   55
#define E_FILECANTWRITE     56
#define IDS_CANNOTQUIT      57
#define E_CANTSAVETOREADONLYFILE 59
#define W_OPENFILEFORREADONLY 60
#define E_PASTEDTEXTTOOLONG    61
#define E_FILESAVE              62

#define IDS_FILTERSPEC    0x300   /* file open/save dlg. filter spec. string */
#define IDS_OPENDLGTITLE  0x301   /* caption of the open dialog              */
#define IDS_SAVEDLGTITLE  0x302   /* caption of the saveas dialog            */
#define IDS_FILTERSPEC2   0x303   /* second filter spec string               */
#define IDS_MERGEDLGTITLE 0x304   /* caption of the merge dialog             */
#define IDS_LINKTITLE     0x305      /* Caption of the link repair dialog         */
#define IDS_FILTERSPEC3   0x306
#define IDS_VALIDATE      0x307   /* Validate check box text */
#define IDS_OBJNAME       0x308   /* Used to construct OLE object names */
#define IDS_UPDATEEMBOBJECT 0x309
#define IDS_RETRYAFTERINSERT    0x30a
#define IDS_OBJECTCMD     0x30b
#define IDS_PICTUREFMT    0x30c
#define IDS_BITMAPFMT     0x30d
#define IDS_UPDATELINK    0x30e
#define IDS_OBJECTFMT     0x30f
#define IDS_PopupVerbs    0x310
#define IDS_SingleVerb    0x311
#define IDS_Edit          0x312

extern char szCardfile[];
extern char szCardClass[];
extern char szWindows[];
extern char szDevice[];

extern char NotEnoughMem[200];
extern char szHelpFile[];
extern char szUntitled[];
extern char szWarning[];
extern char szNote[];
extern char szFileExtension[];

extern int iTopCard;

extern char szServerFilter[];         /* default filter spec. for servers  */
extern char szFilterSpec[];           /* default filter spec. */
extern char szLinkCaption[];
extern char szLastDir[];

HANDLE hFind;
char szCustFilterSpec[];              /* buffer for custom filters created */
extern BOOL fValidDB;                 /* Validate on/off in Save dialog box */

#include "funcs.h"