
/*	program ENGLISH.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/


#include <windows.h>
#include <stdlib.h>
#include <stdio.h>   /* drv = 1.1st file 2.def 3.A	*/
#include <ctype.h>
#include "advent.h"
#include "advdec.h"

#include "english.h"
#include "database.h"
#include <strsafe.h>

/*
    Analyze a two word sentence
*/
int english()
{

    char    *msg;
    int type1, type2, val1, val2;

    g_iVerb = g_iObject = g_iMotion = 0;
    type2 = val2 = -1;
    type1 = val1 = -1;
    msg = "bad grammar...";

    getwords();

    if (!(*word1))
        return(0);      /* ignore whitespace	*/
    if (!analyze(word1, &type1, &val1)) /* check word1	*/
        return(0);      /* didn't know it	*/

    if (type1 == 2 && val1 == SAY)
    {
        g_iVerb = SAY; /* repeat word & act upon if..	*/
        g_iObject = 1;
        return(1);
    }

    if (*word2)
    {
        #ifdef DEBUG1
        printf("ENGLISH.CPP english calling analyze\n");
        #endif
        if (!analyze(word2, &type2, &val2))
            return(0);  /* didn't know it	*/
    }

        /* check his grammar */
    if ((type1 == 3) && (type2 == 3) && \
        (val1 == 51) && (val2 == 51))
    {
        outwords();
        return(0);
    }
    else if (type1 == 3)
    {
        rspeak(val1);
        return(0);
    }
    else if (type2 == 3)
    {
        rspeak(val2);
        return(0);
    }
    else if (type1 == 0)
    {
        if (type2 == 0)
        {
            printf("%s\n", msg);
            return(0);
        }
        else
            g_iMotion = val1;
    }
    else if (type2 == 0)
        g_iMotion = val2;
    else if (type1 == 1)
    {
        g_iObject = val1;
        if (type2 == 2)
            g_iVerb = val2;
        if (type2 == 1)
        {
            printf("%s\n", msg);
            return(0);
        }
    }
    else if (type1 == 2)
    {
        g_iVerb = val1;
        if (type2 == 1)
            g_iObject = val2;
        if (type2 == 2)
        {
            printf("%s\n", msg);
            return(0);
        }
    }
    else
        bug(36);
    return(1);
}


/*
        Routine to analyze a word.
*/
int analyze(char *word, int *type, int *value)
{
    int wordval, msg;

    /* make sure I understand */
    if ((wordval = vocab(word, 0)) == -1)
    {
        // nope - I don't recognize this word.  Print out an error message
        switch (rand() % 3)
        {
        case 0:
            msg = 60;
            break;
        case 1:
            msg = 61;
            break;
        default:
            msg = 13;
        }
        rspeak(msg);
        return(0);
    }
    *type = wordval/1000;
    *value = wordval%1000;

    #ifdef DEBUG1
    printf("ENGLISH.cpp analyze type = %d\n", *type);
    printf("ENGLISH.cpp analyze value = %d\n", *value);
    printf("ENGLISH.cpp analyze word = %s\n", word);
    #endif

    return(1);
}

/*
    retrieve input line (max 80 chars), convert to lower case
     & rescan for first two words (max. WORDSIZE-1 chars).
*/
void getwords()
{
    char    words[80], *wptr;
    
    fputc('>', stdout);
    word1[0] = word2[0] = '\0';
    fgets(words, 80, stdin);
    wptr = words;
    while (*wptr = tolower(*wptr))
        ++wptr;
    sscanf_s(words, "%19s %19s", word1,WORDSIZE, word2, WORDSIZE);
#ifdef DEBUG1
        printf("WORD1 = %s, WORD2 = %s\n", word1, word2);
#endif
    return;
}

/*
    output adventure word list (g_iMotion/0xxx & g_iVerb/2xxx) only
    6 words/line pausing at 20th line until keyboard active
*/
void outwords()
{
    int i, j, line;
    char    words[80];

    j = line = 0;
    for (i = 0; i < MAXWC; ++i)
    {
        if ((g_wc[i].acode < 1000) || ((g_wc[i].acode < 3000) && \
                                     (g_wc[i].acode > 1999)))
        {
            printf("%-12s", g_wc[i].aword);
            if ((++j == 6) || (i == MAXWC-1))
            {
                j = 0;
                fputc('\n', stdout);
                if (++line == 20)
                {
                    line = 0;
                    printf("\n\007Enter <RETURN>");
                    printf(" to continue\n\n");
                    fgets(words, 80, stdin);
                }
            }
        }
    }
}