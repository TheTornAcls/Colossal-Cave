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
    gettrav
    -------
    Fills the global 'travel' array with possible travel options for a given location.
    For the specified location index (iloc), this function reads the encoded travel data
    from the 'cave' array and decodes each entry into destination, verb, and condition fields
    in the 'travel' array. The function stops when it encounters a -1 sentinel value.
    The last entry in 'travel' is set to indicate the end of the array.

    Parameters:
        iloc - The location index (1-based) for which to fill the travel array.

    Side Effects:
        - Modifies the global 'travel' array with the decoded travel options for the given location.
        - May print debug information if compiled with DEBUG and dbugflg is set.
        - Calls bug(33) if the travel array overflows.
*/
void gettrav(int locationIndex)
{
    struct trav* travelPtr;
    long int* cavePtr;
    // Each entry in cave[locationIndex - 1] encodes travel information as a single integer:
    // Bits 0-2:   Condition (tcond)   = value % 1000
    // Bits 3-5:   Verb (tverb)        = (value / 1000) % 1000
    // Bits 6-8:   Destination (tdest) = (value / 1000000) % 1000
    // This section decodes each field for the travel array.
    for (travelPtr = &travel[0], cavePtr = cave[locationIndex - 1]; *cavePtr != -1; travelPtr++, cavePtr++) {
        if (travelPtr >= &travel[MAXTRAV]) bug(33);
        long int encodedValue = *cavePtr;
        // Decode travel condition (lowest 3 digits)
        travelPtr->tcond = (int)(encodedValue % 1000);
        // Decode verb (next 3 digits)
        travelPtr->tverb = (int)((encodedValue / 1000) % 1000);
        // Decode destination (next 3 digits)
        travelPtr->tdest = (int)((encodedValue / 1000000) % 1000);
    }
    travelPtr->tdest = -1; /* end of array */
#ifdef DEBUG
    if (dbugflg)
        for (travelPtr = &travel[0]; travelPtr->tdest != -1; ++travelPtr)
            printf("cave[%d] = %d %d %d\n",
                locationIndex, travelPtr->tdest, travelPtr->tverb, travelPtr->tcond);
#endif
}

/*
    rdupto
    ------
    Reads characters from a file up to a specified delimiter character. Optionally prints the characters or stores them in a string. Handles newlines and carriage returns, and can insert a leading newline if prespace is set.

    Parameters:
        fdi      - Pointer to the input FILE to read from.
        uptoc    - Character to read up to (delimiter, not included in output).
        print    - If nonzero, prints the characters to stdout; otherwise, stores them in 'string'.
        string   - Pointer to a buffer to store the string (if print is zero); ignored if print is nonzero.
        prespace - If nonzero, prints a newline before the output (only on the first character).

    Returns:
        true if the delimiter was found, false if EOF was reached first.

    Side Effects:
        - Reads from the input file stream 'fdi'.
        - May print to stdout if 'print' is nonzero.
        - May write to the buffer pointed to by 'string' if 'print' is zero.
*/
bool rdupto(FILE * fdi, char uptoc, char print, char* string, int prespace)
{
    int    c;

    while ((c = fgetc(fdi)) != uptoc) {
        if (prespace) {
            putchar('\n');
            prespace = 0;
        }
        if (c == EOF)
            return false;
        if (c == '\r')
            continue;
        if (print)
            putchar(c);
        else
            *string++ = (char)c;
    }
    if (!print)
        * string = '\0';
    return true;
}

/*
    rdskip
    ------
    Reads and skips over a specified character in a file a given number of times. Optionally rewinds the file before skipping. Used to position the file pointer for reading specific data.

    Parameters:
        fdi    - Pointer to the input FILE to read from.
        skipc  - Character to skip over.
        n      - Number of times to skip the character.
        rewind - If nonzero, rewinds the file to the beginning before skipping.

    Side Effects:
        - Reads from and may reposition the input file stream 'fdi'.
        - May call bug(31) or bug(32) and exit on file errors or EOF.
*/
void rdskip(FILE * fdi, char skipc, int n, char rewind)
{
    int    c;

    if (rewind)
        if (fseek(fdi, 0l, 0) == -1)
            bug(31);
    while (n--)
        while ((c = fgetc(fdi)) != skipc)
            if (c == EOF)
                bug(32);
}

/*
    yes
    ---
    Prompts the player for a yes or no answer, processes the response, and prints appropriate messages. Handles input, converts it to lowercase, and checks for 'yes' or 'no'.

    Parameters:
        msg1 - Message number to print before prompting (0 for none).
        msg2 - Message number to print if the answer is 'yes' (0 for none).
        msg3 - Message number to print if the answer is 'no' (0 for none).

    Returns:
        true if the player answers 'yes', false if 'no'.

    Side Effects:
        - Prints messages to the player using rspeak().
        - Reads input from stdin.
        - May update the game state depending on how the result is used by the caller.
*/
bool yes(int msg1, int msg2, int msg3)
{
    char            answer[80];
    int    y = -1;

    if (msg1)
        rspeak(msg1);
    while (y == -1) {
        char* i;
        size_t   l;
        putchar('>');
        fflush(stdout); 
        fgets(answer, 80, stdin);
        for (i = answer; *i != '\n'; i++) *i = (char)tolower(*i);
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
    rspeak
    ------
    Prints a location or general message to the player, using a message number to look up the text in the message file. Handles special case for message 54 (prints "ok.").

    Parameters:
        msg - The message number to print.

    Side Effects:
        - Prints the message to stdout.
        - Seeks and reads from the message file (fd4) using the global index array 'idx4'.
        - May print debug information if DEBUG and dbugflg are set.
*/
void rspeak(int msg)
{
    if (msg == 54)
        printf("ok.\n");
    else {
#ifdef DEBUG
        if (dbugflg)
            printf("Seek loc msg #%d @ %ld\n", msg, idx4[msg]);
#endif
        fseek(fd4, idx4[msg - 1], 0);
        rdupto(fd4, '#', 1, 0, 0);
    }
    return;
}

/*
    pspeak
    ------
    Prints an item-specific message for a given state from the item message file ("advent3.txt"). Seeks to the correct message for the item and state, and prints it to the player. Optionally prints a leading newline if prespace is set.

    Parameters:
        item     - The item number for which to print the message.
        state    - The state index for the message (which variant to print).
        prespace - If nonzero, prints a newline before the message.

    Side Effects:
        - Seeks and reads from the item message file (fd3) using the global index array 'idx3'.
        - Calls rdskip() and rdupto() to position and print the message.
        - Prints the message to stdout.
*/
void pspeak(int item, int state, int prespace)
{
    fseek(fd3, idx3[item - 1], 0);
    rdskip(fd3, '/', state + 2, 0);
    rdupto(fd3, '/', 1, 0, prespace);
}

/*
    desclg
    ------
    Prints a long location description from the location description file ("advent1.txt") for the given location index. Seeks to the correct offset and prints the description up to the next '#' character.

    Parameters:
        iloc - The location index for which to print the long description.

    Side Effects:
        - Seeks and reads from the location description file (fd1) using the global index array 'idx1'.
        - Calls rdupto() to print the description to stdout.
*/
void desclg(int iloc)
{
    fseek(fd1, idx1[iloc - 1], 0);
    rdupto(fd1, '#', 1, 0, 0);
}

/*
    vocab
    -----
    Looks up a word in the adventure's vocabulary table and returns its associated code if found, or -1 if not found.

    Parameters:
        word - Pointer to the word (string) to look up.

    Returns:
        The code associated with the word if found, or -1 if not found.

    Side Effects:
        - Calls binary() to perform the lookup.
        - Reads the global vocabulary array 'wc'.
*/
int vocab(char* word)
{
    int    v1;

    if ((v1 = binary(word, wc, MAXWC)) >= 0)
        return(wc[v1].acode);
    else
        return(-1);
}

/*
    binary
    ------
    Performs a binary search for a word in a sorted vocabulary table and returns its index if found, or -1 if not found.

    Parameters:
        w        - Pointer to the word (string) to search for.
        wctable  - Array of vocabulary entries to search (struct wac[]).
        maxwc    - Number of entries in the vocabulary array.

    Returns:
        The index of the word in the array if found, or -1 if not found.

    Side Effects:
        - None (does not modify global variables or state).
*/
int binary(char* w, struct wac wctable[], int maxwc)
{
    int             check;
    int    lo, hi, mid;

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
    Determines if the current location is dark (i.e., not lit and the lamp is not present or not on).

    Parameters:
        None (uses global state for location, lamp, and conditions)

    Returns:
        true if the location is dark, false otherwise.

    Side Effects:
        - Reads the global variables 'cond', 'loc', 'prop', and 'LAMP' to determine darkness.

*/
bool dark(void)
{
    return(!(cond[loc] & LIGHT) &&
        (!prop[LAMP] ||
            !here(LAMP)));
}

/*
    here
    ----
    Determines if a specified item is present at the current location or is being carried by the player.

    Parameters:
        item - The item number to check for presence.

    Returns:
        true if the item is at the current location or being carried, false otherwise.

    Side Effects:
        - Reads the global variables 'place' and 'loc' to check item location.
        - Calls toting() to check if the item is being carried.
*/
bool here(int item)
{
    return(place[item] == loc || toting(item));
}

/*
    toting
    ----__
    Determines if a specified item is currently being carried by the player.

    Parameters:
        item - The item number to check.

    Returns:
        true if the item is being carried (i.e., its place is -1), false otherwise.

    Side Effects:
        - Reads the global variable 'place' to check the item's status.
*/
bool toting(int item)
{
    return(place[item] == -1);
}

/*
    Determines if a specified location causes a forced move (i.e., the player must move upon entering).

    Parameters:
        atloc - The location index to check.

    Returns:
        true if the location is a forced-move location, false otherwise.

    Side Effects:
        - Reads the global variable 'cond' to check the forced move flag for the location.
*/
bool forced(int atloc)
{
    return((cond[atloc] & FORCED) != 0);
}

/*
    Generates a random integer in the specified inclusive range [low, high]. Uses the standard C library rand() function and ensures uniform distribution.

    Parameters:
        low  - The lower bound of the range (inclusive).
        high - The upper bound of the range (inclusive).

    Returns:
        A random integer between low and high, inclusive.

    Side Effects:
        - Calls the standard library rand() function, which may update the global random seed/state.
*/
int rrand(int low, int high)
{
    unsigned range = (unsigned)high - (unsigned)low + 1;
    unsigned partition = ((unsigned)RAND_MAX + 1) / range;
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
bool pct(int x)
{
    return(rrand(0, 99) < x);
}

/*
        Routine to tell if player is on
        either side of a two sided object.
*/
bool at(int item)
{
    return(place[item] == loc || fixed[item] == loc);
}

/*
        Routine to destroy an object
*/
void dstroy(int obj)
{
    move(obj, 0);
}

/*
        Routine to move an object
*/
void move(int obj, int where)
{
    int    from;

    from = (obj < MAXOBJ) ? place[obj] : fixed[obj - MAXOBJ];
    if (from > 0 && from <= 300)
        carry(obj, from);
    drop(obj, where);
}

/*
        Juggle an object
        currently a no-op
*/
void juggle(int iloc)
{
}

/*
        Routine to carry an object
*/
void carry(int obj, int where)
{
    if (obj < MAXOBJ) {
        if (place[obj] == -1)
            return;
        place[obj] = -1;
        ++holding;
    }
}

/*
        Routine to drop an object
*/
void drop(int obj, int where)
{
    if (obj < MAXOBJ) {
        if (place[obj] == -1)
            --holding;
        place[obj] = where;
    }
    else
        fixed[obj - MAXOBJ] = where;
}

/*
        routine to move an object and return a
        value used to set the negated prop values
        for the repository.
*/
int put(int obj, int where, int pval)
{
    move(obj, where);
    return((-1) - pval);
}
/*
        Routine to check for presence
        of dwarves..
*/
int dcheck(void)
{
    int    i;

    for (i = 1; i < (DWARFMAX - 1); ++i)
        if (dloc[i] == loc)
            return(i);
    return(0);
}

/*
        Determine liquid in the bottle
*/
int liq(void)
{
    int    i, j;
    i = prop[BOTTLE];
    j = -1 - i;
    return(liq2(i > j ? i : j));
}

/*
        Determine liquid at a location
*/
int liqloc(int iloc)
{
    if (cond[iloc] & LIQUID)
        return(liq2(cond[iloc] & WATOIL));
    else
        return(liq2(1));
}

/*
        Convert 0 to WATER
                 1 to nothing
                 2 to OIL
*/
int liq2(int pbottle)
{
    return((1 - pbottle) * WATER + (pbottle >> 1) * (WATER + OIL));
}

/*
        Fatal error routine
*/
void bug(int n)
{
    printf("Fatal error number %d\n", n);
    exit(1);
}