/* function prototypes */

/* card.c */
LRESULT EditWndProc( HWND hwnd, 
                     UINT message, 
                     WPARAM wParam, 
                     LPARAM lParam);

BOOL CardChar( int ch);

void CardPaint(HDC hDC);

void DeleteCard(int iCard);

int AddCurCard( void);

BOOL SaveCurrentCard(int iCard);

void SetCurCard(int iCard);

void SetEditText(char *pText);

BOOL ScrollCards(HWND hWindow, 
                 int cmd, 
                 int pos);

void DoCutCopy(int event);

void DoPaste(int event);

void PaintNewHeaders(HWND hWnd);

/* dial.c */
BOOL fnDial(HWND hDB,
            unsigned message,
            WORD wParam,
            LONG lParam);

void DoDial(PSTR pchNumber);

/* dragdrop.c */
void EndDragDrop(void);

void DoDragDrop( HWND hwnd, 
                 HDROP hdrop, 
                 BOOL fCard);

/* file.c */
int TextRead( HANDLE fh,
              char *szBuf);

void AppendExtension(char *pName,
                     char *pBuf);

int WriteCurCard( PCARDHEADER pCardHead,
                  PCARD pCard,
                  char *pText);

int ReadCurCardData( PCARDHEADER pCardHead,
                     PCARD pCard,
                     char *pText);

LPSTR MyStrChr( LPSTR str, 
                int ch);

LPSTR MyStrrChr( LPSTR str, 
                 int ch);

LPSTR FileFromPath(LPSTR lpStr);

BOOL ExpandHdrs(int n);

/* find.c */
BOOL SearchLine( LPSTR lpLine,
                 char *szPattern);

BOOL SearchLineAt( LPSTR lpLine,
                   char *szPattern);

void DoGoto( char *pBuf);

void ForwardSearch(void);

void ReverseSearch( void);
/* indb.c */
BOOL DlgProc( HWND hDB,
              unsigned message,
              WORD wParam,
              LONG lParam);

BOOL fnLinksDlg( HWND hDlg,
                 unsigned message,
                 WORD wParam,
                 LONG lParam);

int fnInvalidLink( HWND hDlg,
                   unsigned message,
                   WORD wParam,
                   LONG lParam);

BOOL HookProc( HWND hDlg,
               unsigned msg,
               WORD wParam,
               LONG lParam);

/* index.c */
LRESULT CALLBACK IndexWndProc( HWND hwnd,
                               UINT message,
                               WPARAM wParam,
                               LPARAM lParam);

LRESULT CALLBACK CardWndProc( HWND hwnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam);

void SizeListWindow( void);

void ReleaseClientDoc(void);

void IndexMouse( HWND hWindow, 
                 int message, 
                 WORD wParam, 
                 POINT pt);

void IndexPaint( HDC hDC);

/* init.c */
//void GetOldData( HANDLE hInstance);

BOOL InitInstance( HANDLE hInstance,
                   LPSTR lpszCommandLine,
                   int cmdShow);

BOOL IndexInit( void);

BOOL OleInit( HANDLE hInstance);

//int CallBack( LPOLECLIENT lpclient,
//              OLE_NOTIFICATION flags,
//              LPOLEOBJECT lpObject);

/* input.c */
DWORD IndexInput( HWND hWindow, 
                  int event);

char * PutUpDB( int idb);

void UpdateMenu( void);

int MapPtToCard( POINT pt);

DWORD OleMenu( int event);

void OleMenuItemFix( HMENU hMenu, 
                     int Mode);

void ScrollIndexHorz( HWND hWindow, 
                      int cmd, 
                      int pos);

void ScrollIndexVert( HWND hWindow, 
                      int cmd, 
                      int pos);

void MenuFileNew( void);

void MakeTempFile( void);

void InitPhoneList( HWND hWindow, 
                    int iStartCard);

int CheckForBusyObjects( void);

/* insert.c */
int CALLBACK InsertObjectDlgProc( HWND hDlg, 
                                  UINT msg, 
                                  WPARAM wParam, 
                                  LPARAM lParam) ;

void InsertObject( void);

//  main.c 
//int WinMain(
//                  HINSTANCE hInstance, 
//                  HINSTANCE hPrevInstance,
//                  LPSTR lpszCommandLine,
//                  int cmdShow);

void SetNumOfCards( void);

void FixBounds( LPRECT lprc);

BOOL ProcessMessage( HWND hwndFrame, 
                     HANDLE hAccTable);

int Scale( int coord, 
           int s1, 
           int s2);

BOOL IndexOkError( int strid);

BOOL MergeStrings( LPSTR lpszSrc, 
                   LPSTR lpszMerge, 
                   LPSTR lpszDst);

void MakeBlankCard( void);

void SetCaption( void);

void BuildCaption(
                 char *pchBuf);

void IndexWinIniChange(
                      void);

BOOL BuildAndDisplayMsg( int idError, 
                         char szString[]);

LONG Myatol( char *szNum);

BOOL Myisdigit( char c);

short TranslateString( char *src);

/* object.c */
void BMMouse( HWND hWindow,
              int message,
              WORD wParam,
              POINT pt);

BOOL BMKey( WORD wParam);

/* special.c */
INT_PTR CALLBACK PasteSpecialDlgProc( HWND hDlg, 
                                      UINT message, 
                                      WPARAM wParam, 
                                      LPARAM lParam);

void DoPasteSpecial( void);

/* picture.c */
void PicDelete( PCARD pCard);

BOOL PicRead( PCARD pCard, 
              HANDLE fh, 
              BOOL fOld);

BOOL PicWrite( PCARD pCard, 
               HANDLE fh, 
               BOOL fForceOld);

BOOL PicDraw( PCARD pCard, 
              HDC hDC, 
              BOOL fAtOrigin);

void PicCutCopy( PCARD pCard, 
                 BOOL fCut);

void PicPaste( PCARD pCard, 
               BOOL fPaste,
               WORD ClipFormat);

HBITMAP MakeObjectCopy( PCARD pCard);

DWORD ReadOldStream( LPCARDSTREAM lpStream, 
                     LPSTR lpstr, 
                     DWORD cb);

DWORD ReadStream( LPCARDSTREAM lpStream, 
                  LPSTR lpstr, 
                  DWORD cb);

DWORD WriteStream( LPCARDSTREAM lpStream, 
                   LPSTR lpstr, 
                   DWORD cb);

DWORD PosStream( LPCARDSTREAM lpStream, 
                 LONG pos);

DWORD lread( int hFile, 
             void *pBuffer, 
             DWORD dwBytes);

DWORD lwrite( int hFile, 
              void *pBuffer, 
              DWORD dwBytes);

HBITMAP MakeBitmapCopy(
                      HBITMAP hbmSrc, 
                      PBITMAP pBitmap);

WORD OleError( HRESULT olestat);

BOOL GetNewLinkName( HWND hwndOwner, 
                     PCARD pCard);

void PicSaveUndo( PCARD pCard);

void ErrorMessage( int id);

void WaitForObject( LPOLEOBJECT lpObject);

void PicCreateFromFile( LPSTR szPackageClass, 
                        LPSTR szDropFile, 
                        BOOL fLink);

void Hourglass( BOOL fOn);

BOOL EditingEmbObject( PCARD pCard);

int UpdateEmbObject( PCARD pCard,
                     int Flags);

BOOL InsertObjectInProgress( void);

void DoSetHostNames( LPOLEOBJECT lpObject,
                     OBJECTTYPE otObject);

void DeleteUndoObject( void);

/* print.c */
void FreePrintHandles(void);

void PrinterSetupDlg(HWND hwnd);

long atopix( char *ptr, 
             int pix_per_in);

HDC GetPrinterDC( void);

HDC SetupPrinting(void);

void FinishPrinting( HDC hPrintDC);

void PrintList(void);

void PrintCards(int count);

void PrintCurCard( HDC hPrintDC,
                   HDC hMemoryDC,
                   int xPos,
                   int yPos,
                   PCARDHEADER pCardHead,
                   PCARD   pCard,
                   HWND    hWnd);

int fnAbortProc( HDC hPrintDC,  
                 int iReserved);

int fnAbortDlgProc( HWND hwnd,
                    unsigned msg,
                    WORD wParam,
                    LONG lParam);

int PageSetupDlgProc( HWND hwnd,
                      unsigned msg,
                      WORD wParam,
                      LONG lParam);

void PrintError( int iError);

/* read.c */
int CheckCardfileSignature( int fh);

BOOL MaybeSaveFile( int fSystemModal);

void MenuFileOpen( void);

int OpenNewFile(char szFile[]);

int DoOpen(char *szFile);

void MenuFileMerge(void);

/* register.c */
void GetClassId( HWND hwnd, 
                 LPSTR lpstrClass);

int MakeFilterSpec( LPSTR lpstrClass, 
                    LPSTR lpstrExt, 
                    LPSTR lpstrFilterSpec);

/* write.c */
BOOL MyGetSaveFileName( char *szFile);

int WriteCardFile( char *pName,     
                   BOOL fOldFormat);

/* indos2.asm */
void RepMov( void *lpDest, 
             void *lpSrc, 
             WORD n);

void RepMovDown( void *lpDest, 
                 void *lpSrc, 
                 WORD n);

/* indos.asm */
int DosCall(void);

int Frename( char *src, 
             char *dst);

BOOL Fdelete( char *src);

int mylread( HANDLE fh, 
             void *buf, 
             int n);

int mylwrite( HANDLE fh, 
              void *buf, 
              int n);

int myread( HANDLE fh, 
            void *buf, 
            int n);

int mywrite( HANDLE fh, 
             void *buf, 
             int n);

long mylmul( int n1,
             int n2);

