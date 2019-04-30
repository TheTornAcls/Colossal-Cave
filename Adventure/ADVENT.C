/*      program ADVENT.C                                        */

#include        <stdio.h>
#include        <ctype.h>
#include        <stdlib.h>
#include        <string.h>
#include        <stdbool.h>
#include        "advent.h"      /* #define preprocessor equates */
#include        "advep.h"       /* define all functions         */
#include        "advword.h"     /* definition of "word" array   */
#include        "advcave.h"     /* definition of "cave" array   */
#include        "advtext.h"     /* definition of "text" arrays  */
#include        "advdec.h"      /* definition of global data    */

int main(register int argc, register char** argv)
{
    int	rflag;		/* user restore request option	*/

    rflag = 0;
    dbugflg = 0;
#if defined(MVS)
    szFilePath[0] = '\0';
#else
    {
        int i;
        strcpy_s(szFilePath, FILENAME_MAX, argv[0]);
        for (i = strlen(szFilePath) - 1; i >= 0 && szFilePath[i] != '\\'; i--);
        szFilePath[i + 1] = '\0';
    }
#endif
    while (--argc > 0) {
        ++argv;
        if (**argv != '-' && **argv != '/')
            break;
        switch (tolower(argv[0][1])) {
        case 'r':
            ++rflag;
            continue;
#if ! defined(NDEBUG)
        case 'd':
            ++dbugflg;
            continue;
#endif
        default:
            printf("unknown flag: %c\n", argv[0][1]);
            continue;
        }                               /* switch       */
    }                                       /* while        */
#if ! defined(NDEBUG)
    if (dbugflg < 2)
        dbugflg = 0;    /* must request three times     */
#endif
    opentxt();
    initplay();
    if (rflag)
        restore();
    else if (yes(65, 1, 0)) {
        limit = 1000;
        ++hinttaken;
    }
    else
        limit = 330;
    saveflg = 0;
    srand(511);                             /* seed random  */
    while (!saveflg)
        turn();
    if (saveflg)
        saveadv();
    fclose(fd1); fclose(fd2); fclose(fd3); fclose(fd4);
    return 0;                               /* exit = ok    */
}                                               /* main         */


/* ************************************************************ */

/*
        Initialization of adventure play variables
*/
void initplay(void)
{
    static const short int icond[MAXLOC] =
        /*   0 */{ 0, 2053, 2049, 2053, 2053, 2049, 2049, 2053,   33,    1,
        /*  10 */      1,    0,    0,   64,    0,    0,   16,    0,    0,  128,
        /*  20 */     16,   16,   16,    0,    6,    0,   16,    0,    0,    0,
        /*  30 */      0,   16,   16,    0,    0,    0,    0,    0,    4,    0,
        /*  40 */     16,    0,  256,  256,  256,  256,  264,  264,  264,  256,
        /*  50 */    256,  256,  256,  264,  256,  264,    0,    8,    0,   16,
        /*  60 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  70 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    3,
        /*  80 */    256,  256,  264,    0,    0,    8,  264,  256,    0,   16,
        /*  90 */     16,    0,    0,    0,    0,    4,    0,    0,    0,  512,
        /* 100 */    513,    0,    0,    0,    0,    0,    0,    0, 1024,    0,
        /* 110 */      0,    0,    0,    4,    0,    1,    1,    0,    0,    0,
        /* 120 */      0,    0,    8,    8,    8,    8,    8,    8,    8,    8,
        /* 130 */      8,    0,    0,    0,    0,    0,    0,    0,    0,    0 };

    static const short int iplace[MAXOBJ] =
        /*   0 */{ 0,    3,    3,    8,   10,   11,    0,   14,   13,   94,
        /*  10 */     96,   19,   17,  101,  103,    0,  106,    0,    0,    3,
        /*  20 */      3,    0,    0,  109,   25,   23,  111,   35,    0,   97,
        /*  30 */      0,  119,  117,  117,    0,  130,    0,  126,  140,    0,
        /*  40 */     96,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  50 */     18,   27,   28,   29,   30,    0,   92,   95,   97,  100,
        /*  60 */    101,    0,  119,  127,  130,    0,    0,    0,    0,    0,
        /*  70 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  80 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  90 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0 };

    static const short int ifixed[MAXOBJ] =
        /*   0 */{ 0,    0,    0,    9,    0,    0,    0,   15,    0,   -1,
        /*  10 */      0,   -1,   27,   -1,    0,    0,    0,   -1,    0,    0,
        /*  20 */      0,    0,    0,   -1,   -1,   67,   -1,  110,    0,   -1,
        /*  30 */     -1,  121,  122,  122,    0,   -1,   -1,   -1,   -1,    0,
        /*  40 */     -1,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  50 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  60 */      0,    0,  121,    0,    0,    0,    0,    0,    0,    0,
        /*  70 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  80 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        /*  90 */      0,    0,    0,    0,    0,    0,    0,    0,    0,    0 };


    static const short int iactmsg[33] =
        /*   0 */{ 0,   24,   29,    0,   33,    0,   33,   38,   38,   42,
        /*  10 */     14,   43,  110,   29,  110,   73,   75,   29,   13,   59,
        /*  20 */     59,  174,  109,   67,   13,    0,   90,  195,  146,  110,
        /*  30 */     13,   13,  155 };

    static const short int idloc[DWARFMAX] =
    { 0, 19, 27, 33, 44, 64, 114 };
    turns = 0;

    /* initialize location status array */
    memcpy(cond, icond, (sizeof(short int)) * MAXLOC);

    /* initialize object locations */
    memcpy(place, iplace, (sizeof(short int)) * MAXOBJ);

    /* initialize second (fixed) locations */
    memcpy(fixed, ifixed, (sizeof(short int)) * MAXOBJ);

    /* initialize default verb messages */
    memcpy(actmsg, iactmsg, (sizeof(short int)) * 33);

    /* initialize various flags and other variables */
    memset(visited, 0, (sizeof(short int)) * MAXLOC);
    memset(prop, 0, (sizeof(short int)) * MAXOBJ);
    memset(&prop[50], 0xff, (sizeof(short int)) * (MAXOBJ - 50));
    wzdark = false;
    closed = closing = holding = detail = 0;
    limit = 100;
    tally = 15;
    tally2 = 0;
    newloc = 1;
    loc = oldloc = oldloc2 = 2;
    knfloc = 0;
    chloc = 114;
    chloc2 = 140;
    /*      dloc[DWARFMAX-1] = chloc                                */
    memcpy(dloc, idloc, (sizeof(short int)) * DWARFMAX);
    memset(odloc, 0, (sizeof(short int)) * DWARFMAX);
    dkill = 0;
    memset(dseen, 0, (sizeof(short int)) * DWARFMAX);
    clock1 = 30;
    clock2 = 50;
    panic = 0;
    bonus = 0;
    numdie = 0;
    daltloc = 18;
    lmwarn = 0;
    foobar = 0;
    dflag = 0;
    gaveup = false;
    saveflg = 0;
    hinttaken = 0;
    hintavail = HINT;
    memset(hintloc, 0, (sizeof(short int) * (MAXHINT + 1)));
    testbr = 2;
    return;
}

/*
        Open advent?.txt files
*/
void open1(FILE * *pfd, char* szName)
{
    char szFileName[FILENAME_MAX];
    errno_t rc;
    strcpy_s(szFileName, FILENAME_MAX, szFilePath);
    strcat_s(szFileName, FILENAME_MAX, szName);
    rc = fopen_s(pfd, szFileName, "r");

    if (NULL == *pfd)
    {
        printf("Sorry, I can't open %s\n", szFileName);
        exit(1);
    }
}
void opentxt(void)
{
    open1(&fd1, FD1);
    open1(&fd2, FD2);
    open1(&fd3, FD3);
    open1(&fd4, FD4);
}

/*
                save adventure game
*/
void saveadv(void)
{
    register char* sptr;
    FILE* savefd;
    char            username[MAXNAME];

    do {
        printf("What do you want to name the saved game? ");
        fflush(stdout);
        gets_s(username, MAXNAME);
    } while (*username == '\0');
    if (sptr = strchr(username, '.'))
    {
        *sptr = '\0';           /* kill extension       */
    }
    if (strlen(username) > 8)
    {
        username[8] = '\0'; /* max 8 char filename */
    }
    strcat_s(username, MAXNAME, ".adv");
    fopen_s(&savefd, username, SAVEMODE);
    if (savefd == NULL)
    {
        printf("Sorry, I can't create the file...%s\n", username);
        exit(1);
    }
    if (fwrite(&saverec, sizeof saverec, 1, savefd) != 1) {
        printf("Write error on save file...%s\n", username);
        exit(1);
    }
    if (fclose(savefd) == -1) {
        printf("Sorry, I can't close the file...%s\n", username);
        exit(1);
    }
    printf("OK -- \"C\" you later...\n");
}

/*
        restore saved game handler
*/
void restore(void)
{
    char            username[MAXNAME];
    FILE* restfd;
    register char* sptr;
    register int    savedebug = dbugflg;

    do {
        printf("What is the name of the saved game? ");
        fflush(stdout);
        gets_s(username, MAXNAME);
    } while (*username == '\0');

    if (sptr = strchr(username, '.'))
        * sptr = '\0';           /* kill extension       */
    if (strlen(username) > 8)
        username[8] = '\0'; /* max 8 char filename */
    strcat_s(username, MAXNAME, ".adv");
    fopen_s(&restfd, username, RESTMODE);
    if (restfd == NULL) {
        printf("Sorry, I can't open the file...%s\n", username);
        exit(1);
    }
    if (fread(&saverec, sizeof saverec, 1, restfd) != 1) {
        printf("Read error on save file...%s\n", username);
        exit(1);
    }
    if (fclose(restfd) == -1) {
        printf("Warning -- can't close save file...%s\n", username);
    }
    dbugflg |= savedebug; loc = 0; putchar('\n');
}
