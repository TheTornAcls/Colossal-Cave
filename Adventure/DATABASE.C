/*      program DATABASE.C                                      */

#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <ctype.h>
#include        <stdbool.h>
#include        "advent.h"
#include        "advep.h"
#include        "advdec.h"

/*
        Routine to fill travel array for a given location
*/
void gettrav(int iloc)
{
        register struct trav *i;
        register long int *j;
        for (i = &travel[0], j = cave[iloc - 1]; *j != -1; i++, j++) {
            if (i >= &travel[MAXTRAV]) bug(33);
            i->tcond = (int) (*j % 1000);
            i->tverb = (int) ((*j / 1000) % 1000);
            i->tdest = (int) ((*j / 1000000) % 1000);
        }
        i->tdest = -1; /* end of array */
#if ! defined(NDEBUG)
        if (dbugflg)
               for (i = &travel[0]; i->tdest != -1; ++i)
                       printf("cave[%d] = %d %d %d\n",
                              iloc, i->tdest, i->tverb, i->tcond);
#endif
}

/*
        Function to scan a file up to a specified
        point and either print or return a string.
*/
int rdupto(FILE *fdi, char uptoc, char print, char *string, register int prespace)
{
        register int    c;

        while ((c = fgetc(fdi)) != uptoc) {
                if (prespace) {
                        putchar ('\n');
                        prespace = 0;
                }
                if (c == EOF)
                        return(0);
                if (c == '\r')
                        continue;
                if (print)
                        putchar (c);
                else
                        *string++ = (char) c;
        }
        if (!print)
                *string = '\0';
        return(1);
}

/*
        Function to read a file skipping
        a given character a specified number
        of times, with or without repositioning
        the file.
*/
void rdskip(FILE *fdi, char skipc, register int n, char rewind)
{
        register int    c;

        if (rewind)
                if (fseek(fdi, 0l, 0) == -1)
                        bug(31);
        while (n--)
                while ((c = fgetc(fdi)) != skipc)
                        if (c == EOF)
                                bug(32);
}

/*
        Routine to request a yes or no answer to a question.
*/
bool yes(register int msg1, int msg2, int msg3)
{
    char            answer[80];
    register int    y = -1;

    if (msg1)
            rspeak(msg1);
    while (y == -1) {
        register char *i;
        register int   l;
        putchar ('>');
        fflush (stdout); fgets(answer, 80, stdin);
        for (i = answer; *i != '\n'; i++) *i = (char) tolower(*i);
        l = i - answer;
        if (l == 0)
            rspeak(89);
        else if ((l <= 2) && (memcmp(answer, "no", l) == 0)) {
            if (msg3)
                rspeak(msg3);
            y = 0;
        }
        else if ((l <= 3) && (memcmp(answer, "yes", l) == 0)) {
            if (msg2)
                rspeak(msg2);
            y = 1;
        }
        else
            rspeak(89);
    }
    return (1 == y);
}

/*
        Print a location description from "advent4.txt"
*/
void rspeak(register int msg)
{
        if (msg == 54)
                printf("ok.\n");
        else {
#if ! defined(NDEBUG)
                if (dbugflg)
                        printf("Seek loc msg #%d @ %ld\n", msg, idx4[msg]);
#endif
                fseek(fd4, idx4[msg - 1], 0);
                rdupto(fd4, '#', 1, 0, 0);
        }
        return;
}

/*
        Print an item message for a given state from "advent3.txt"
*/
void pspeak(register int item, register int state, int prespace)
{
        fseek(fd3, idx3[item - 1], 0);
        rdskip(fd3, '/', state+2, 0);
        rdupto(fd3, '/', 1, 0, prespace);
}

/*
        Print a long location description from "advent1.txt"
*/
void desclg(register int iloc)
{
        fseek(fd1, idx1[iloc - 1], 0);
        rdupto(fd1, '#', 1, 0, 0);
}

/*
        Print a short location description from "advent2.txt"
*/
void descsh(register int iloc)
{
        fseek(fd2, idx2[iloc - 1], 0);
        rdupto(fd2, '#', 1, 0, 0);
}

/*
        look-up vocabulary word in lex-ordered table.
        word is the word to look up.
*/
int vocab(char *word)
{
        register int    v1;

        if ((v1 = binary(word, wc, MAXWC)) >= 0)
                return(wc[v1].acode);
        else
                return(-1);
}

int binary(char *w, struct wac wctable[], int maxwc)
{
        int             check;
        register int    lo, hi, mid;

        lo = 0;
        hi = maxwc - 1;
        while (lo <= hi) {
                mid = (lo + hi) / 2;
                if ((check = strcmp(w, wctable[mid].aword)) < 0)
                        hi = mid - 1;
                else if (check > 0)
                        lo = mid + 1;
                else
                        return(mid);
        }
        return(-1);
}


/*
        Utility Routines
*/

/*
        Routine to test for darkness
*/
bool dark(void)
{
        return(!(cond[loc] & LIGHT) &&
                (!prop[LAMP] ||
                !here(LAMP)));
}

/*
        Routine to tell if an item is present.
*/
int here(register int item)
{
        return(place[item] == loc || toting(item));
}

/*
        Routine to tell if an item is being carried.
*/
int toting(register int item)
{
        return(place[item] == -1);
}

/*
        Routine to tell if a location causes
        a forced move.
*/
int forced(register int atloc)
{
        return((cond[atloc] & FORCED) != 0);
}

/*
        Routine to supply a random number in a selected range
*/
int rrand(int low, int high)
{
        unsigned range = (unsigned) high - (unsigned) low + 1;
        unsigned partition = ((unsigned) RAND_MAX + 1) / range;
        int      discard_limit = partition * range - 1;
        int      x;
        int      y = -1;

        do
                if ((x = rand()) <= discard_limit)
                        y = low + x / partition;
        while (y == -1);
        return y;
}

/*
        Routine true x% of the time.
*/
int pct(register int x)
{
        return(rrand(0, 99) < x);
}

/*
        Routine to tell if player is on
        either side of a two sided object.
*/
int at(register int item)
{
        return(place[item] == loc || fixed[item] == loc);
}

/*
        Routine to destroy an object
*/
void dstroy(register int obj)
{
        move(obj, 0);
}

/*
        Routine to move an object
*/
void move(int obj, int where)
{
        int    from;

        from = (obj<MAXOBJ) ? place[obj] : fixed[obj - MAXOBJ];
        if (from>0 && from<=300)
                carry(obj, from);
        drop(obj, where);
}

/*
        Juggle an object
        currently a no-op
*/
void juggle(register int iloc)
{
}

/*
        Routine to carry an object
*/
void carry(register int obj, register int where)
{
        if (obj<MAXOBJ){
                if (place[obj] == -1)
                        return;
                place[obj]=-1;
                ++holding;
        }
}

/*
        Routine to drop an object
*/
void drop(register int obj, register int where)
{
        if (obj<MAXOBJ) {
                if (place[obj] == -1)
                        --holding;
                place[obj]=where;
        }
        else
                fixed[obj-MAXOBJ]=where;
}

/*
        routine to move an object and return a
        value used to set the negated prop values
        for the repository.
*/
int put(int obj, int where, register int pval)
{
        move(obj, where);
        return((-1)-pval);
}
/*
        Routine to check for presence
        of dwarves..
*/
int dcheck(void)
{
        register int    i;

        for (i =1; i < (DWARFMAX-1); ++i)
                if (dloc[i] == loc)
                        return(i);
        return(0);
}

/*
        Determine liquid in the bottle
*/
int liq(void)
{
        register int    i, j;
        i=prop[BOTTLE];
        j=-1-i;
        return(liq2(i>j ? i : j));
}

/*
        Determine liquid at a location
*/
int liqloc(register int iloc)
{
        if (cond[iloc]&LIQUID)
                return(liq2(cond[iloc]&WATOIL));
        else
                return(liq2(1));
}

/*
        Convert 0 to WATER
                 1 to nothing
                 2 to OIL
*/
int liq2(register int pbottle)
{
        return((1-pbottle)*WATER+(pbottle>>1)*(WATER+OIL));
}

/*
        Fatal error routine
*/
void bug(register int n)
{
        printf("Fatal error number %d\n", n);
        exit(1);
}
