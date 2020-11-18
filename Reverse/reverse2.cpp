#include <stdio.h>
#include <windows.h>
#include "reverse.h"

cReverse::cReverse()
{
   pTail = NULL;
   pHead = NULL;
}

cReverse::~cReverse()
{   
    PNODE pCount;
    PNODE pCurrent;

    pCount = pHead;

    while (NULL != pCount)
    {
        pCurrent = pCount;
        pCount = pCurrent->pNext;

        LocalFree(pCurrent);

    }
}

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

void cReverse::ShoveSpace(PNODE pNewNode)
{

    pNewNode->pNext = pHead;
    pHead = pNewNode;

    pTail = pHead;

    return;
}

void cReverse::ShoveChar(PNODE pNewNode)
{

    if (NULL != pTail)
    {
        if (pTail->character == ' ')
        {  
            ShoveSpace(pNewNode);
        }
        else
        {
            pNewNode->pNext = pTail->pNext;
            pTail->pNext = pNewNode;
        }
    
    }
    else
    {
        pHead = pNewNode;
    }
    pTail = pNewNode;


    return;
}

void cReverse::DumpString( )
{
    PNODE pCount = pHead;
    while (NULL != pCount)
    {
        printf("%c",pCount->character);
        pCount = pCount->pNext;
    }
    printf("\n");

    return;
}

