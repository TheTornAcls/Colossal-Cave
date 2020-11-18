#include <stdio.h>
#include <windows.h>
#include "FindFirst.h"

ALPHABET Alphabet[ALPHABETSIZE];
ALPHABET HeadPTR, TailPTR;          // using a structure just makes my life easier


int main(int argc, char *argv[])
{
    LPSTR lpstrChar;
    INT   iCount;


    // did they provide us a string
    if (argc < 2)
    {
        printf("You need to provide a string\n");
        return 0;
    }

    // Initialize the Alphabet
    for (iCount =0;iCount < ALPHABETSIZE; iCount++)
    {
        // Set both of our pointers to NULL
        Alphabet[iCount].pPrev = NULL;
        Alphabet[iCount].pNext = NULL;

        // Initialize the character value
        Alphabet[iCount].character = iCount;
    }

    // Intialize the pointers
    HeadPTR.pNext = &TailPTR;
    TailPTR.pPrev = &HeadPTR;

    lpstrChar = argv[1];

    // Walk through the list
    while ( *lpstrChar != '\0')
    {
        // OK double check to make sure the character is in our Alphabet
        if ( (*lpstrChar >= 'a') && ( *lpstrChar <= 'z') )
        {
            // have we removed this character
            // does the structure's pPrev pointer point to itself??
            if (Alphabet[*lpstrChar - 'a'].pPrev != &Alphabet[*lpstrChar- 'a'])
            {
                // no we haven't removed this character
                if (Alphabet[*lpstrChar - 'a'].pPrev == NULL)
                {
                    // We haven't seen this character
                    AddTo(lpstrChar);
                }
                else
                {
                    // We have seen this character before.... remove it from the list
                    Remove(lpstrChar);
                }
            }
        }
        else
        {
            printf("Skipping Character %c\n",*lpstrChar);
        }


        lpstrChar++;
    }

    // if the Head Pointer doesn't point to NULL then we have a non-duplicate character in the string
    if (HeadPTR.pNext != &TailPTR)
    {
        printf("The first non-duplicate character is: %c\n", HeadPTR.pNext->character + 'a');
    }
    else
    {
        printf("there are no non-duplicate characters\n");
    }

    return 1;
}

// Add the character to the double-linked list
VOID AddTo(LPSTR lpstrCurrent)
{
    // Just to make our life easier
    BYTE bLocation = *lpstrCurrent - 'a';

    // insert the element at the end of a double linked list before the tail pointer.
    TailPTR.pPrev->pNext = &Alphabet[bLocation];
    Alphabet[bLocation].pPrev = TailPTR.pPrev;
    Alphabet[bLocation].pNext = &TailPTR;
    TailPTR.pPrev = &Alphabet[bLocation];

    return;
}          

// Remove the character from the doubly linked list
// Set both of its pointers to point to itself
VOID Remove(LPSTR lpstrCurrent)
{
    BYTE bLocation = *lpstrCurrent - 'a';
    // Fix up the pointers
    Alphabet[bLocation].pPrev->pNext = Alphabet[bLocation].pNext;
    Alphabet[bLocation].pNext->pPrev = Alphabet[bLocation].pPrev;

    // Set the .pPrev pointer to point to current location - the check for skipping
    Alphabet[bLocation].pPrev = &Alphabet[bLocation];

    return;
}
