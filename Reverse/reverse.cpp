#include <stdio.h>
#include <windows.h>
#include "reverse.h"

cReverse::cReverse()
{
   pTail = NULL;
   nHead.pNext = NULL;
}

// destroy the created stack/queue
cReverse::~cReverse()
{   
    PNODE pCount;
    PNODE pCurrent;

    pCount = nHead.pNext;

    while (NULL != pCount)
    {
        pCurrent = pCount;
        pCount = pCurrent->pNext;

        LocalFree(pCurrent);

    }
}

// Create a stack/queue element and initialzed it with the provided character
PNODE cReverse::CreateNode(CHAR cNew)
{
    PNODE pNewNode;

    pNewNode = (PNODE) LocalAlloc(LPTR,sizeof(NODE));
    if (NULL != pNewNode)
    {
        pNewNode->pNext = NULL;
        pNewNode->character = cNew;
    }
    return pNewNode;
}

// PUBLIC: add a new character to the Stack
bool cReverse::AddChar(CHAR cNew)
{
    PNODE pNewNode;

    pNewNode = CreateNode(cNew);

    if (NULL == pNewNode)
    {
        return false;
    }


    if (' ' == cNew)
    {
        ShoveSpace(pNewNode);
    }
    else
    {
        ShoveChar(pNewNode);
    }

    return true;
}

// add a space to the beginning of the String Stack and reset the pointers so the next word is added
// to the begining of the string.
void cReverse::ShoveSpace(PNODE pNewNode)
{

    // we are adding a space
    // we need to reset the tail pointer and the head's next pointer.
    // This means we have finished placing a word on the stack
    pNewNode->pNext = nHead.pNext;
    nHead.pNext = pNewNode;

    pTail = &nHead;

    return;
}

// Add a character to the end of the word Queue
void cReverse::ShoveChar(PNODE pNewNode)
{

    if (NULL != pTail)
    {
        // Add the next character to the Word Queue
        pNewNode->pNext = pTail->pNext;
        pTail->pNext = pNewNode;
    }
    else
    {
        // The list was empty
        nHead.pNext = pNewNode;
    }

    // make sure the tail pointer points to the last element in the Word Queue
    pTail = pNewNode;


    return;
}

// Starting from the first character print out the new string.
void cReverse::DumpString( )
{
    PNODE pCount = nHead.pNext;
    while (NULL != pCount)
    {
        printf("%c",pCount->character);
        pCount = pCount->pNext;
    }
    printf("\n");

    return;
}

