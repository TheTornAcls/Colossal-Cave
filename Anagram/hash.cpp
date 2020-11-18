#include <stdio.h>
#include <windows.h>

#include "hash.h"

cHash::cHash()
{
    INT iCount;
    // initialize the hash table to all nulls

    for (iCount =0; iCount < HASHVAL; iCount++)
    {
        HashTable[iCount] = NULL;
    }

}

cHash::~cHash()
{
    INT iCount;
    // initialize the hash table to all nulls

    for (iCount =0; iCount < HASHVAL; iCount++)
    {
        PNODE pCurrent;
        PNODE pNext;

        // If we have have elements in our hash table.
        // REmove them
        pCurrent = HashTable[iCount];
        while (NULL != pCurrent)
        {
            pNext = pCurrent->pNext;
            LocalFree(pCurrent);
            pCurrent = pNext;

        }

    }

}

BOOL cHash::Add(LPSTR lpstrNew)
{
    INT iHash;
    PNODE pIndex, pLast, pNew;

    // create the new node
    pNew = (PNODE)LocalAlloc(LPTR, sizeof(NODE));
    if (NULL == pNew)
    {
        return FALSE;
    }
    pNew->lpstrString = lpstrNew;
    pNew->pNext = NULL;

    // create the hash Value
    iHash = ComputeHash(lpstrNew);
    
    // iS that location empty
    if (NULL == HashTable[iHash])
    {
        HashTable[iHash] = pNew;
    }
    else
    {
        // Location not empty add to the end
        pLast = HashTable[iHash];

        pIndex = pLast->pNext;

        while (NULL != pIndex)
        {
            pLast = pIndex;
            pIndex = pIndex->pNext;
        }

        pLast->pNext = pNew;

    }


    return TRUE;

}

UINT cHash::ComputeHash(LPSTR lpstrNew)
{
    UINT iHash =1;

    while (*lpstrNew != '\0')
    {
        // Mutliply the character values togeter
        iHash *= *lpstrNew;

        // Cool property of Modulo arthimetic
        //  (a * c) % b == (a % b) * (c % b) 
        iHash = iHash % HASHVAL;

        //printf(" %c = %d\n", *lpstrNew, *lpstrNew);
        lpstrNew++;

        // NOTE: HASHVAL should be a PRIME number where NONE of your characters is a
        // multiple of it.
        // Otherwise  (character % HASHVAL) == 0, and you will get a TON of collisions.
        // :-) My first HASHVAL was 101 and I got a ton of collisions.  The
        // ASCII value of 'e' is 101.  A really really bad choice.

    }

    return (iHash);
}

VOID cHash::DumpHash()
{
    INT iCount;
    PNODE pIndex;
    INT iNumAnagrams = 0;

    for (iCount =0; iCount < HASHVAL; iCount++)
    {
        pIndex = HashTable[iCount];

        // TODO: print out all the non anagram strings first, then the Anagrams

        if (NULL !=pIndex)
        {
            // did we find any Anagrams (did we add more into this location?)
            if (pIndex->pNext == NULL)
            {
                printf("%s does not have any Anagrams\n\n",pIndex->lpstrString);
            }
            else
            {
                printf("The %d Anagrams are:",++iNumAnagrams);
                while (NULL != pIndex)
                {
                    printf(" %s", pIndex->lpstrString);
                    pIndex = pIndex->pNext;
                }
                printf("\n\n");
            }
        }
    }
}


