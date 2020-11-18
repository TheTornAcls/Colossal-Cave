#ifndef _FINDFIRST
#define _FINDFIRST

#define ALPHABETSIZE 26

typedef struct _ALPHABET *AlphabetPTR;

typedef struct _ALPHABET
{
    AlphabetPTR pPrev;
    CHAR character;
    AlphabetPTR pNext;
} ALPHABET;

VOID AddToHash(CHAR cAdd);
BOOL SearchHash(CHAR cSearch);
INT RemoveChar(LPSTR lpSearch, LPSTR lpRemoveList);

#endif  // _FINDFIRST
