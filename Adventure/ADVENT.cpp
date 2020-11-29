
/*	program ADVENT.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h"		*/


#include    <windows.h>
#include	<stdio.h>
#include    <ctype.h>
#include	<string.h>
#include    <stdlib.h>
#include	"advent.h"	/* #define preprocessor equates	*/
#include	"advword.h"	/* definition of "word" array	*/
#include	"advcave.h"	/* definition of "cave" array	*/
#include	"advtext.h"	/* definition of "text" arrays	*/
#include	"advdef.h"

#define 	setmem(a,b,c)	memset(a,c,b)

void opentxt();
void initplay();
void restore();
void saveadv();

#include "database.h"
#include "turn.h"
#include "printing.h"

#include    <strsafe.h>

extern cPrinting g_Printing;

int main(int argc, char *argv[])
{
    int rflag;      /* user restore request option	*/
    
    g_Printing.ConOutput("Starting Colossal Cave\n");

    rflag = 0;
    dbugflg = 0;
    while (--argc > 0)
    {
        ++argv;
        if (**argv !=  '-')
            break;
        switch (tolower(argv[0][1]))
        {
        case 'r':
            ++rflag;
            continue;
        case 'd':
            ++dbugflg;
            continue;
        default:
            g_Printing.ConOutputError("unknown flag: %c\n", argv[0][1]);
            continue;
        }               /* switch	*/
    }                   /* while	*/
    if (dbugflg < 2)
        dbugflg = 0;    /* must request three times	*/
    opentxt();
    initplay();
    if (rflag)
        restore();
    else if (yes(65, 1, 0))
        limit = 1000;
    else
        limit = 330;
    saveflg = 0;
    srand(511);             /* seed random	*/
    while (!saveflg)
        turn();
    if (saveflg)
        saveadv();
    fclose(fd1);
    fclose(fd2);
    fclose(fd3);
    fclose(fd4);
    exit(0);                /* exit = ok	*/
}                       /* main		*/

/* ************************************************************	*/

/*
    Initialize integer arrays with sscanf
*/
void scanint(int *pi, char *str)
{

    while (*str)
    {
        if ((sscanf_s(str, "%d,", pi++)) < 1)
            bug(41);    /* failed before EOS	*/
        while (*str++ != ',')   /* advance str pointer	*/
            ;
    }
    return;
}

/*
    Initialization of adventure play variables
*/
void initplay()
{
    g_iTurns = 0;

    /* initialize location status array */
    setmem(cond, (sizeof(int))*MAXLOC, 0);
    scanint(&cond[1], "5,1,5,5,1,1,5,17,1,1,");
    scanint(&cond[13], "32,0,0,2,0,0,64,2,");
    scanint(&cond[21], "2,2,0,6,0,2,");
    scanint(&cond[31], "2,2,0,0,0,0,0,4,0,2,");
    scanint(&cond[42], "128,128,128,128,136,136,136,128,128,");
    scanint(&cond[51], "128,128,136,128,136,0,8,0,2,");
    scanint(&cond[79], "2,128,128,136,0,0,8,136,128,0,2,2,");
    scanint(&cond[95], "4,0,0,0,0,1,");
    scanint(&cond[113], "4,0,1,1,");
    scanint(&cond[122], "8,8,8,8,8,8,8,8,8,");

    /* initialize object locations */
    setmem(place, (sizeof(int))*MAXOBJ, 0);
    scanint(&place[1], "3,3,8,10,11,0,14,13,94,96,");
    scanint(&place[11], "19,17,101,103,0,106,0,0,3,3,");
    scanint(&place[23], "109,25,23,111,35,0,97,");
    scanint(&place[31], "119,117,117,0,130,0,126,140,0,96,");
    scanint(&place[50], "18,27,28,29,30,");
    scanint(&place[56], "92,95,97,100,101,0,119,127,130,");

    /* initialize second (fixed) locations */
    setmem(fixed, (sizeof(int))*MAXOBJ, 0);
    scanint(&fixed[3], "9,0,0,0,15,0,-1,");
    scanint(&fixed[11], "-1,27,-1,0,0,0,-1,");
    scanint(&fixed[23], "-1,-1,67,-1,110,0,-1,-1,");
    scanint(&fixed[31], "121,122,122,0,-1,-1,-1,-1,0,-1,");
    scanint(&fixed[62], "121,-1,");

    /* initialize default verb messages */
    scanint(actmsg, "0,24,29,0,33,0,33,38,38,42,14,");
    scanint(&actmsg[11], "43,110,29,110,73,75,29,13,59,59,");
    scanint(&actmsg[21], "174,109,67,13,147,155,195,146,110,13,13,");

    /* initialize various flags and other variables */
    setmem(visited, (sizeof(int))*MAXLOC, 0);
    setmem(prop, (sizeof(int))*MAXOBJ, 0);
    setmem(&prop[50], (sizeof(int))*(MAXOBJ-50), 0xff);
    wzdark = closed = closing = holding = detail = 0;
    limit = 100;
    tally = 15;
    tally2 = 0;
    newloc = 3;
    g_iLoc = oldloc = oldloc2 = 1;
    knfloc = 0;
    chloc = 114;
    chloc2 = 140;
/*	dloc[DWARFMAX-1] = chloc				*/
    scanint(dloc, "0,19,27,33,44,64,114,");
    scanint(odloc, "0,0,0,0,0,0,0,");
    dkill = 0;
    scanint(dseen, "0,0,0,0,0,0,0,");
    clock1 = 30;
    clock2 = 50;
    panic = 0;
    bonus = 0;
    numdie = 0;
    daltloc = 18;
    lmwarn = 0;
    foobar = 0;
    dflag = 0;
    gaveup = 0;
    saveflg = 0;
    return;
}

/*
    Open advent?.txt files
*/
void opentxt()
{       
    int iError;
    
    iError = fopen_s(&fd1,"advent1.txt", "r");
    if (0 != iError)
    {
        g_Printing.ConOutputError("Yo Sorry, I can't open advent1.txt...\n");
        exit(0);
    }
    iError = fopen_s(&fd2, "advent2.txt", "r");
    if (0 != iError)
    {
        g_Printing.ConOutputError("Sorry, I can't open advent2.txt...\n");
        goto error2;
    }
    iError = fopen_s(&fd3, "advent3.txt", "r");
    if (0 != iError)
    {
        g_Printing.ConOutputError("Sorry, I can't open advent3.txt...\n");
        goto error3;
    }
    iError = fopen_s(&fd4, "advent4.txt", "r");
    if (0 != iError)
    {
        g_Printing.ConOutputError("Sorry, I can't open advent4.txt...\n");
        goto error4;
    }
    
    return;
    
error4:    fclose(fd3);
error3:    fclose(fd2);
error2:    fclose(fd1);
    exit(0);
}

/*
        save adventure game
*/
void saveadv()
{
    char    *sptr;
    FILE    *savefd;
    char    username[13];
    int i;
    int iError;

    g_Printing.ConOutput("What do you want to name the saved game? ");
    scanf_s("%s", username);
    if (sptr = strchr(username, '.'))
        *sptr = '\0';       /* kill extension	*/
    if (strlen(username) > 8)
        username[8] = '\0'; /* max 8 char filename	*/
    //strcat(username, ".adv");
    StringCbCatA(username, 13, ".adv");
    iError = fopen_s(&savefd, username, "wb");
    if (0 != iError)
    {
        g_Printing.ConOutputError("Sorry, I can't create the file...%s\n", username);
        exit(0);
    }

    _putw(g_iTurns,savefd);  /* mh 30-Aug-1988 */
    _putw(g_iLoc,savefd);
    _putw(oldloc,savefd);
    _putw(oldloc2,savefd);
    _putw(newloc,savefd);
    for (i=0;i<MAXLOC;i++) _putw(cond[i],savefd);
    for (i=0;i<MAXOBJ;i++) _putw(place[i],savefd);
    for (i=0;i<MAXOBJ;i++) _putw(fixed[i],savefd);
    for (i=0;i<MAXLOC;i++) _putw(visited[i],savefd);
    for (i=0;i<MAXOBJ;i++) _putw(prop[i],savefd);
    _putw(tally,savefd);
    _putw(tally2,savefd);
    _putw(limit,savefd);
    _putw(lmwarn,savefd);
    _putw(wzdark,savefd);
    _putw(closing,savefd);
    _putw(closed,savefd);
    _putw(holding,savefd);
    _putw(detail,savefd);
    _putw(knfloc,savefd);
    _putw(clock1,savefd);
    _putw(clock2,savefd);
    _putw(panic,savefd);
    for (i=0;i<DWARFMAX;i++) _putw(dloc[i],savefd);
    _putw(dflag,savefd);
    for (i=0;i<DWARFMAX;i++) _putw(dseen[i],savefd);
    for (i=0;i<DWARFMAX;i++) _putw(odloc[i],savefd);
    _putw(daltloc,savefd);
    _putw(dkill,savefd);
    _putw(chloc,savefd);
    _putw(chloc2,savefd);
    _putw(bonus,savefd);
    _putw(numdie,savefd);
    _putw(object1,savefd);
    _putw(gaveup,savefd);
    _putw(foobar,savefd);
    _putw(saveflg,savefd);
    _putw(dbugflg,savefd);

    if (fclose(savefd)  ==  -1)
    {
        g_Printing.ConOutputError("Sorry, I can't close the file...%s\n", username);
        exit(0);
    }
    g_Printing.ConOutput("OK -- \"C\" you later...\n");
}

/*
    restore saved game handler
*/
void restore()
{
    char    username[13];
    FILE    *restfd;
    int i;
    char    *sptr;
    int     iError;

    g_Printing.ConOutput("What is the name of the saved game? ");
    scanf_s("%s", username);
    if (sptr = strchr(username, '.'))
        *sptr = '\0';       /* kill extension	*/
    if (strlen(username) > 8)
        username[8] = '\0'; /* max 8 char filename	*/
    StringCbCat(username,13, ".adv");
    iError = fopen_s(&restfd, username, "rb");
    if (0 != iError)
    {
        g_Printing.ConOutputError("Sorry, I can't open the file...%s\n", \
               username);
        exit(0);
    }

    g_iTurns=_getw(restfd);  /* mh 30-Aug-1988 */
    g_iLoc=_getw(restfd);
    oldloc=_getw(restfd);
    oldloc2=_getw(restfd);
    newloc=_getw(restfd);
    for (i=0;i<MAXLOC;i++) cond[i]=_getw(restfd);
    for (i=0;i<MAXOBJ;i++) place[i]=_getw(restfd);
    for (i=0;i<MAXOBJ;i++) fixed[i]=_getw(restfd);
    for (i=0;i<MAXLOC;i++) visited[i]=_getw(restfd);
    for (i=0;i<MAXOBJ;i++) prop[i]=_getw(restfd);
    tally=_getw(restfd);
    tally2=_getw(restfd);
    limit=_getw(restfd);
    lmwarn=_getw(restfd);
    wzdark=_getw(restfd);
    closing=_getw(restfd);
    closed=_getw(restfd);
    holding=_getw(restfd);
    detail=_getw(restfd);
    knfloc=_getw(restfd);
    clock1=_getw(restfd);
    clock2=_getw(restfd);
    panic=_getw(restfd);
    for (i=0;i<DWARFMAX;i++) dloc[i]=_getw(restfd);
    dflag=_getw(restfd);
    for (i=0;i<DWARFMAX;i++) dseen[i]=_getw(restfd);
    for (i=0;i<DWARFMAX;i++) odloc[i]=_getw(restfd);
    daltloc=_getw(restfd);
    dkill=_getw(restfd);
    chloc=_getw(restfd);
    chloc2=_getw(restfd);
    bonus=_getw(restfd);
    numdie=_getw(restfd);
    object1=_getw(restfd);
    gaveup=_getw(restfd);
    foobar=_getw(restfd);
    saveflg=_getw(restfd);
    dbugflg=_getw(restfd);

    if (fclose(restfd)  ==  -1)
    {
        g_Printing.ConOutputError("Warning -- can't close save file...%s\n", \
               username);
    }
}
