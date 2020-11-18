#include "windows.h"
#include "ole2.h"

#define OLE_PLAY        OLEVERB_PRIMARY
#define OLE_EDIT        (OLEVERB_PRIMARY+1)

#define	PASTELINK	    119
#define PLAY            150
#define UPDATE          151
#define FREEZE          152
#define EDIT            153
#define CLONE		    134
#define COPYFROMLINK	135
#define	LINKSDIALOG	    136
#define EDITOBJECT      137

/* menu IDs used for OLE_VERBs, from OLE_VERB to OLE_VERBMAX */
#define OLE_VERB        1000
#define OLE_VERBMAX     OLE_VERB+25

#define IDM_OPENFILE    138
#define IDM_LINKFILE    139
#define IDM_EMBEDFILE   140


/* Ensure that these aren't the same as DM_ messages (!) */
#define WM_REDRAWDIALOG (WM_USER + 0x100)

/* New error messages */
extern HWND hwndError;
void ErrorMessage(int x);
#define	RETRY		2

#define E_BITMAP_COPY_FAILED                    0x100
#define E_BOUNDS_QUERY_FAILED                   0x101
#define E_GET_FROM_CLIPBOARD_FAILED             0x102
#define E_FAILED_TO_FREEZE                      0x103
#define E_FAILED_TO_LAUNCH_SERVER               0x104
#define E_FAILED_TO_READ_CARD                   0x105
#define E_FAILED_TO_UPDATE                      0x106
#define E_NEW_FILE_NOT_READABLE                 0x107
#define	E_FAILED_TO_DELETE_OBJECT               0x108
#define	E_SERVER_BUSY                           0x109
#define	E_ASYNCHRONOUS_OPERATION_FAILED         0x10a
#define	E_FAILED_TO_UPDATE_LINK                 0x10b
#define	E_IMPROPER_LINK_OPTIONS                 0x10c
#define E_FAILED_TO_RECONNECT_OBJECT            0x10d
#define	E_DRAG_DROP_FAILED                      0x10e
#define E_CLIPBOARD_LOCKED                      0x10f
#define E_CREATEOBJECTFAILED                    0x110
#define E_FLOPPY_WITH_SOURCE_REMOVED            0x111
#define E_FLOPPY_WITH_SOURCE_REMOVED2           0x112
#define E_PRINT_SETUP_ERROR                     0x113

#define W_IMPROPER_LINK_OPTIONS                 0x200
#define W_SAVING_OLD_FORMAT                     0x201
#define	W_STATIC_OBJECT                         0x202
#define	W_FAILED_TO_CLONE_UNDO                  0x203
#define W_FAILED_TO_NOTIFY                      0x204
#define W_FREEBUSYSERVER                        0x205
#define W_REPLACEOPENOBJECT                     0x206
#define W_DELETEOPENOBJECT                      0x208
#define W_WILLDELETEOBJECTS                     0x209

/* 
 * Constant IDs used in the Object Properties... dialog
 */
#define IDD_WHAT        0x0100
#define IDD_CLASSID     0x0101
#define IDD_AUTO        0x0102
#define IDD_MANUAL      0x0103
#define IDD_PLAY        PLAY
#define IDD_FREEZE      FREEZE
#define IDD_UPDATE      UPDATE
#define IDD_EDIT        EDIT

#define IDD_DOCINFO     0x0104
#define IDD_ITEMINFO    0x0105
#define	IDD_CHANGE	    0x0106
#define	IDD_LINK        0x0107
#define IDD_LINKDONE    0x0108

/* Constants */
#define DRAG_EMBED  2               /* Ctrl + Drag */
#define DRAG_OPEN   4               /* Shift + Drag */
#define DRAG_LINK   6               /* Ctrl + Shift + Drag */


typedef enum { EMBEDDED, LINK, STATIC } OBJECTTYPE;

#define	CMAXCLASSNAME	50

typedef struct _CARDSTREAM {
    LPOLESTREAMVTBL    lpstbl;
    HANDLE          hobjStream;         // stream handle
    int             fh;                 // file handle
} CARDSTREAM, *LPCARDSTREAM;

typedef struct _FAKEBITMAP {		/* fbmp */
    DWORD	ulVersion;
    DWORD	ulWhat;
    DWORD	cbName;
    char	szName[7];
    DWORD	res[3];
    BITMAP	bm;
} FAKEBITMAP;

extern HCURSOR          hcurOLE;
extern BOOL		fOLE;
extern FAKEBITMAP	vfbmp;
// TODO extern LPOLECLIENT	lpclient;
extern LPOLECLIENTSITE	lpclient;
extern LPCARDSTREAM	lpStream;
extern LPOLEOBJECT	lpObjectUndo;
extern OBJECTTYPE	otObjectUndo;
extern UINT vcfLink;
extern UINT vcfOwnerLink;
extern UINT vcfNative;
extern int		cOleWait;	/* Count of "open" OLE transactions */
extern HWND		hwndLinkWait;
extern BOOL		fOldCardfile;
// TODO extern OLESTATUS        oleloadstat;
// TODO extern LHCLIENTDOC      lhcdoc;
extern	char		szObjectName[];
extern  char            szObjFormat[];
extern  char            szPStatic[];
extern  char            szPStdFile[];
extern BOOL		fCreateFromFile;
// TODO extern OLESTATUS OleStatusCallBack;

extern DWORD (*pfOldRead)  (LPOLESTREAM, LPSTR, DWORD);
extern DWORD (*pfNewRead)  (LPOLESTREAM, LPSTR, DWORD);
