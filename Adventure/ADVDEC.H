/*      header ADVDEC.H                                         *\
\*      GLOBAL declarations for adventure                       */


/*
        Database variables
*/
#if !defined (WORDDEF)
extern  struct  wac wc[];       /* see ADVWORD.H                */
#endif

#if !defined (CAVEDEF)
extern  long int *cave[];       /* see ADVCAVE.H                */
#endif

#if !defined (TEXTDEF)
extern  long int idx1[];        /* see ADVTEXT.H                */
extern  long int idx2[];        /* see ADVTEXT.H                */
extern  long int idx3[];        /* see ADVTEXT.H                */
extern  long int idx4[];        /* see ADVTEXT.H                */
#endif

struct     trav    travel[MAXTRAV];
FILE	   *fd1, *fd2, *fd3, *fd4;
#if !defined (MVS)
char	   szFilePath[FILENAME_MAX];
#endif
short int  actmsg[33];             /* action messages      */

/*
        English variables
*/
int        verb,object,motion;
char       word1[WORDSIZE],word2[WORDSIZE];

/*
        Play variables
*/
int        testbr;
struct {
    int       turns;
    int       loc,oldloc,oldloc2,newloc;      /* location variables */
    short int cond[MAXLOC];         /* location status      */
    short int place[MAXOBJ];        /* object location      */
    short int fixed[MAXOBJ];        /* second object loc    */
    short int visited[MAXLOC];      /* >0 if has been here  */
    short int prop[MAXOBJ];         /* status of object     */
    int       tally,tally2;         /* item counts          */
    int       limit;                /* time limit           */
    int       lmwarn;               /* lamp warning flag    */
    int       wzdark,closing,closed;/* game state flags     */
    int       holding;              /* count of held items  */
    int       detail;               /* LOOK count           */
    int       knfloc;               /* knife location       */
    int       clock1,clock2,panic;  /* timing variables     */
    short int dloc[DWARFMAX];       /* dwarf locations      */
    int       dflag;                /* dwarf flag           */
    short int dseen[DWARFMAX];      /* dwarf seen flag      */
    short int odloc[DWARFMAX];      /* dwarf old locations  */
    int       daltloc;              /* alternate appearance */
    int       dkill;                /* dwarves killed       */
    int       chloc,chloc2;         /* chest locations      */
    int       bonus;                /* to pass to end       */
    int       numdie;               /* number of deaths     */
    int       object1;              /* to help intrans.     */
    int       gaveup;               /* 1 if he quit early   */
    int       foobar;               /* fie fie foe foo...   */
    int       saveflg;              /* if game being saved  */
    int       dbugflg;              /* if game is in debug  */
    int       hinttaken;            /* minus 1 for each hint */
    int       hintavail;            /* masks of hints still available */
    short int hintloc[MAXHINT+1]; /* moves expended hanging around hint area */
    } saverec;
#define turns     saverec.turns
#define loc       saverec.loc
#define oldloc    saverec.oldloc
#define oldloc2 saverec.oldloc2
#define newloc    saverec.newloc
#define cond      saverec.cond
#define place     saverec.place
#define fixed     saverec.fixed
#define visited saverec.visited
#define prop      saverec.prop
#define tally     saverec.tally
#define tally2    saverec.tally2
#define limit     saverec.limit
#define lmwarn    saverec.lmwarn
#define wzdark    saverec.wzdark
#define closing saverec.closing
#define closed    saverec.closed
#define holding saverec.holding
#define detail    saverec.detail
#define knfloc    saverec.knfloc
#define clock1    saverec.clock1
#define clock2    saverec.clock2
#define panic     saverec.panic
#define dloc      saverec.dloc
#define dflag     saverec.dflag
#define dseen     saverec.dseen
#define odloc     saverec.odloc
#define daltloc saverec.daltloc
#define dkill     saverec.dkill
#define chloc     saverec.chloc
#define chloc2    saverec.chloc2
#define bonus     saverec.bonus
#define numdie    saverec.numdie
#define object1 saverec.object1
#define gaveup    saverec.gaveup
#define foobar    saverec.foobar
#define saveflg saverec.saveflg
#define dbugflg saverec.dbugflg
#define hinttaken saverec.hinttaken
#define hintavail saverec.hintavail
#define hintloc saverec.hintloc
