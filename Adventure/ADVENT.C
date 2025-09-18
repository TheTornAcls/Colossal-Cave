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

#if defined(_WIN64) 
typedef __int64 INT_PTR;
#else 
typedef int INT_PTR;
#endif

int main(int argc, char** argv)
{
    int rflag;      /* user restore request option   */

    rflag = 0;
    dbugflg = 0;
#if defined(MVS)
    szFilePath[0] = '\0';
#else
    {
        INT_PTR i;
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
#ifdef DEBUG
        case 'd':
            ++dbugflg;
            continue;
#endif
        default:
            printf("unknown flag: %c\n", argv[0][1]);
            continue;
        }                               /* switch       */
    }                                       /* while        */
#ifdef DEBUG
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

/**
 * @brief Initializes the game state and resets all adventure play variables to their starting values.
 *
 * This function sets up the initial conditions for a new game session by:
 * - Initializing arrays that track location status (`cond`), object locations (`place`), fixed object locations (`fixed`), default action messages (`actmsg`), and dwarf locations (`dloc`).
 * - Resetting or zeroing arrays and variables related to visited locations (`visited`), object properties (`prop`), and various game state flags and counters.
 * - Setting default values for game progress variables such as `turns`, `holding`, `detail`, `limit`, `tally`, `tally2`, `newloc`, `loc`, `oldloc`, `oldloc2`, `knfloc`, `chloc`, `chloc2`, `dkill`, `clock1`, `clock2`, `panic`, `bonus`, `numdie`, `daltloc`, `lmwarn`, `foobar`, `dflag`, `gaveup`, `saveflg`, `hinttaken`, `hintavail`, `testbr`.
 * - Zeroing or initializing arrays for dwarf old locations (`odloc`), dwarf seen flags (`dseen`), and hint locations (`hintloc`).
 * - Setting boolean flags such as `wzdark`, `closed`, and `closing` to false.
 *
 * @note
 * This function does not take any parameters.
 *
 * @global
 * The following global variables are modified:
 * - cond, place, fixed, actmsg, visited, prop, wzdark, closed, closing, holding, detail, limit, tally, tally2, newloc, loc, oldloc, oldloc2, knfloc, chloc, chloc2, dloc, odloc, dkill, dseen, clock1, clock2, panic, bonus, numdie, daltloc, lmwarn, foobar, dflag, gaveup, saveflg, hinttaken, hintavail, hintloc, testbr, turns
 */
void initplay(void)
{
    /*
        icond[MAXLOC]:
            Initial status flags for each location in the game. Used to initialize the 'cond' array, which tracks conditions such as darkness, visited status, and other special properties for each location.

        iplace[MAXOBJ]:
            Initial locations for each object in the game. Used to initialize the 'place' array, which determines where each object starts in the game world.

        ifixed[MAXOBJ]:
            Secondary (fixed) locations for certain objects. Used to initialize the 'fixed' array, indicating objects that are immovable or have a special fixed position.

        iactmsg[33]:
            Default message numbers for actions/verbs. Used to initialize the 'actmsg' array, which determines what message to display for specific player actions.

        idloc[DWARFMAX]:
            Initial locations for the dwarves (NPCs). Used to initialize the 'dloc' array, setting the starting positions of the dwarves in the game.
    */
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
    closed = false;
    closing = false;
    holding = detail = 0;
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
void open1(FILE** pfd, char* szName)
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

/**
•   @brief Opens the main adventure text data files required for game operation.
•   •   This function initializes the global file pointers fd1, fd2, fd3, and fd4 by opening the corresponding text files (as defined by FD1, FD2, FD3, and FD4) in read mode. It uses the open1 helper function to construct the full file paths and handle errors if any file cannot be opened. These files contain essential game data such as messages, cave descriptions, and other resources needed during gameplay.
•   •   @details
•   •   Calls open1 for each file pointer and file name constant.
•   •   If any file fails to open, the program prints an error message and exits.
•   •   The files are expected to be present in the directory specified by szFilePath.
•   •   @global
•   •   Modifies the following global variables:
•   •   fd1, fd2, fd3, fd4: Set to the opened file streams for the respective adventure text files.
•   •   Uses the following global variables:
•   •   FD1, FD2, FD3, FD4: Constants containing the file names to open.
•   •   szFilePath: The base directory path where the files are located.
*/
void opentxt(void)
{
    open1(&fd1, FD1);
    open1(&fd2, FD2);
    open1(&fd3, FD3);
    open1(&fd4, FD4);
}

/**
 * @brief Saves the current game state to a user-specified file.
 *
 * This function prompts the user for a filename, appends the ".adv" extension,
 * and writes the current game state (contained in the global variable `saverec`)
 * to the specified file. It ensures the filename is valid, handles file creation,
 * and reports errors if the file cannot be created or written to.
 *
 * @details
 * - Prompts the user for a save game filename (without extension).
 * - Ensures the filename does not exceed 8 characters and appends ".adv".
 * - Opens the file in write mode and saves the current game state.
 * - Handles errors for file creation, writing, and closing, printing messages and exiting on failure.
 *
 * @param None
 *
 * @global
 * - Uses and modifies the following global variables:
 *   - `saverec`: The structure containing the entire current game state, which is written to the save file.
 *
 * @note
 * No parameters are passed to this function. All required data is accessed via global variables.
 */
void saveadv(void)
{
    char* sptr;
    FILE* savefd;
    char  username[MAXNAME];

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

/**
 * @brief Restores the game state from a previously saved file.
 *
 * This function prompts the user for the name of a saved game file, appends the ".adv" extension,
 * and attempts to open and read the saved game state from the file. If successful, it loads the
 * saved data into the global game state structure, effectively restoring the game to the point
 * at which it was saved.
 *
 * @details
 * - Prompts the user for the saved game filename (without extension).
 * - Ensures the filename does not exceed 8 characters and appends ".adv".
 * - Opens the file in read mode and loads the saved state into the global variable `saverec`.
 * - Restores the debug flag (`dbugflg`) to its previous value and resets the player's location.
 * - Handles errors for file opening, reading, and closing, printing messages and exiting on failure.
 *
 * @note
 * This function does not take any parameters.
 *
 * @global
 * - Uses and modifies the following global variables:
 *   - `dbugflg`: Temporarily stores and restores the debug flag.
 *   - `saverec`: Structure holding the entire saved game state, overwritten with loaded data.
 *   - `loc`: Resets the player's location to 0 after restoring.
 */
void restore(void)
{
    char  username[MAXNAME];
    FILE* restfd;
    char* sptr;
    int   savedebug = dbugflg;

    do {
        printf("What is the name of the saved game? ");
        fflush(stdout);
        gets_s(username, MAXNAME);
    } while (*username == '\0');

    if (sptr = strchr(username, '.'))
        *sptr = '\0';           /* kill extension       */
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