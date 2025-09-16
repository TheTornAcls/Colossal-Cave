/*      program ENGLISH.C                                       */


#include        <stdio.h>
#include        <stdlib.h>
#include        <ctype.h>
#include        <stdbool.h>
#include        "advent.h"
#include        "advep.h"
#include        "advdec.h"

/*
    english
    -------
    Parses and analyzes a two-word player command, determines the intended action, and sets the appropriate global variables for verb, object, and motion. Handles grammar checking, special cases (such as SAY and HELP), and prints error messages for invalid input.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads and updates the global variables 'verb', 'object', and 'motion' to represent the parsed command.
        - Reads and updates 'word1' and 'word2' for the input words.
        - Calls getwords() to read input, analyze() to interpret words, and rspeak()/printf() to print messages.
        - May call outwords() to print the word list.
        - May return false for invalid or unrecognized input.
*/
bool english(void)
{
    char* msg;
    int     type1, type2, val1, val2;

    verb = object = motion = 0;
    type2 = val2 = -1;
    type1 = val1 = -1;
    msg = "bad grammar...";

    getwords();

    if (!(*word1))
        return false;              /* ignore whitespace    */
    if (!analyze(word1, &type1, &val1))     /* check word1  */
        return false;              /* didn't know it       */

    if (type1 == 2 && val1 == SAY) {
        verb = SAY;     /* repeat word & act upon if..  */
        object = 1;
        return true;
    }

    if (*word2)
        if (!analyze(word2, &type2, &val2))
            return false;      /* didn't know it       */

/* check his grammar */
    if ((type1 == 3) && (type2 == 3) && \
        (val1 == HELP) && (val2 == HELP)) {
        outwords();
        return false;
    }
    else if (type1 == 3) {
        rspeak(val1);
        return false;
    }
    else if (type2 == 3) {
        rspeak(val2);
        return false;
    }
    else if (type1 == 0) {
        if (type2 == 0) {
            printf("%s\n", msg);
            return false;
        }
        else
            motion = val1;
    }
    else if (type2 == 0)
        motion = val2;
    else if (type1 == 1) {
        object = val1;
        if (type2 == 2)
            verb = val2;
        if (type2 == 1) {
            if ((object == WATER || object == OIL) && at(val2))
                verb = POUR;
            else {
                printf("%s\n", msg);
                return false;
            }
        }
    }
    else if (type1 == 2) {
        verb = val1;
        if (type2 == 1)
            object = val2;
        if (type2 == 2) {
            printf("%s\n", msg);
            return false;
        }
    }
    else
        bug(36);
    return true;
}


/*
    analyze
    -------
    Analyzes a single word from player input, determines its type and value, and returns whether it is recognized. If the word is not recognized, prints an error message.

    Parameters:
        word  - Pointer to the word to analyze (char*)
        type  - Pointer to an int to receive the word type (0=motion, 1=object, 2=verb, 3=special)
        value - Pointer to an int to receive the word value (index or code)

    Returns:
        true if the word is recognized, false otherwise

    Side Effects:
        - May call vocab() to look up the word in the vocabulary.
        - May call rspeak() to print an error message if the word is not recognized.
        - Updates the values pointed to by 'type' and 'value'.
*/
bool analyze(char* word, int* type, int* value)
{
    int    wordval, msg;

    /* make sure I understand */
    if ((wordval = vocab(word)) == -1) {
        switch (rrand(0, 2)) {
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
        return false;
    }
    *type = wordval / 1000;
    *value = wordval % 1000;
    return true;
}

/*
    getwords
    --------
    Reads a line of input from the player, converts it to lowercase, and extracts the first two words into the global variables 'word1' and 'word2'. Handles input up to 80 characters and words up to WORDSIZE-1 characters.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads input from stdin.
        - Updates the global variables 'word1' and 'word2' with the first and second words from the input line.
        - May print a prompt ('>') to the player.
*/
void getwords(void)
{
    char          words[80];
    char* wptr, * wptr2;
    int           j;

    putchar('>');
    word1[0] = word2[0] = '\0';
    fflush(stdout); fgets(words, 80, stdin);
    wptr = words;
    while (*wptr = (char)tolower(*wptr))
        ++wptr;
    for (wptr = words, wptr2 = word1, j = 0;
        (*wptr != '\n') && (*wptr != ' ') && j <= 18;
        wptr++, wptr2++, j++)
        * wptr2 = *wptr;
    *wptr2 = '\0';
    if (j > 18)
        while ((*wptr++ != '\n') && *wptr != ' ');
    while (*++wptr == ' ');
    for (wptr2 = word2, j = 0;
        (*wptr != '\n') && (*wptr != ' ') && j <= 18;
        wptr++, wptr2++, j++)
        * wptr2 = *wptr;
    *wptr2 = '\0';
#ifdef DEBUG
    if (dbugflg)
        printf("WORD1 = %s, WORD2 = %s\n", word1, word2);
#endif
    return;
}

/*
    outwords
    --------
    Prints the list of adventure words (motions and verbs) to the player, six words per line, pausing every 20 lines for user input. Useful for displaying the available commands.

    Parameters:
        None (operates on and updates global game state variables)

    Side Effects:
        - Reads the global array 'wc' for the word list.
        - Prints words to stdout.
        - Pauses for user input every 20 lines.
*/
void outwords(void)
{
    int    i, j, line;
    char            words[80];

    j = line = 0;
    for (i = 0; i < MAXWC; ++i) {
        if ((wc[i].acode < 1000) || ((wc[i].acode < 3000) && \
            (wc[i].acode > 1999))) {
            printf("%-12s", wc[i].aword);
            if ((++j == 6) || (i == MAXWC - 1)) {
                j = 0;
                putchar('\n');
                if (++line == 20) {
                    line = 0;
                    printf("\n\007Enter <RETURN>");
                    printf(" to continue\n\n");
                    fflush(stdout);
                    fgets(words, 80, stdin);
                }
            }
        }
    }
}