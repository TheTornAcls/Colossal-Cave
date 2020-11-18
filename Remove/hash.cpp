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

}

BOOL cHash::Add(CHAR cNew)
{
    INT iHash;
    PNODE pIndex, pLast,pNew;

    iHash = ComputeHash(cNew);
    
    pNew = (PNODE)LocalAlloc(LPTR, sizeof(NODE));
    if (NULL == pNew)
    {
        return FALSE;
    }
    pNew->lpstrString = lpstrNew;
    pNew->pNext = NULL;

    if (NULL == HashTable[iHash])
    {
        HashTable[iHash] = pNew;
    }
    else
    {
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
                            
UINT cHash::ComputeHash(CHAR cHash)
{
    UINT iHash =1;

    iHash = cHash % HASHVAL;


    return (iHash);
}

VOID cHash::Find(CHAR cFind)
{
    INT iCount;
    PNODE pIndex;
    INT iHash;

    iHash = ComputeHash(cNew);

}


