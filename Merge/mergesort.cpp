#include <stdio.h>
#include <windows.h>
#include "mergesort.h"

PNODE Merge(PNODE pFirst, PNODE pSecond)
{
    PNODE pMerge;
    NODE Start;

    pMerge = &Start;

    while ((NULL != pSecond) && (NULL != pFirst))
    {
        if (pFirst->iNumber < pSecond->iNumber)
        {
            pMerge->pNext =  pFirst;
            pFirst = pFirst->pNext;

        }
        else
        {
            pMerge->pNext = pSecond;
            pSecond = pSecond->pNext;
        }
        pMerge = pMerge->pNext;

    } // End of the While Loop

    if (pFirst == NULL)
    {
        pMerge->pNext = pSecond;
    }
    else
    {
        pMerge->pNext = pFirst;
    }

    return Start.pNext;
}

VOID Sort(PNODE pList)
{
    PNODE pEnd = NULL;

    PNODE pSearch = pList;
    INT iTemp;

    while (pEnd != pList)
    {
        while (pEnd != pSearch->pNext)
        {
            if (pSearch->iNumber > pSearch->pNext->iNumber)
            {
                iTemp = pSearch->iNumber;
                pSearch->iNumber = pSearch->pNext->iNumber;
                pSearch->pNext->iNumber = iTemp;
            }
            pSearch = pSearch->pNext;
        }
        pEnd = pSearch;
        pSearch = pList;
    }

    return;

}

VOID DumpList(PNODE pList)
{
    while (NULL != pList)
    {
        printf("%i ",pList->iNumber);
        pList = pList->pNext;
    }
    printf("\n");

    return;
}
