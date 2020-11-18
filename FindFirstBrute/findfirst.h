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

char FindWithSpace(char * cInput);
char FindBrute(char * cInput);

VOID AddTo(LPSTR lpstrCurrent);
VOID Remove(LPSTR lpstrCurrent);

#endif  // _FINDFIRST
