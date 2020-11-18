#include <windows.h>
#include <stdio.h>

#include "pop.h"
#include "binary.h"
#include "connect.h"

cBinTree::cBinTree()
{
    //printf("cBinTree Contructor\n");
}

cBinTree::~cBinTree()
{
    //printf("cBinTree DeContructor\n");

}

void cBinTree::Identification()
{
    printf("cBinTree::");
}

void cBinTree::Descend(CONNECTPTR pCurrent, CONNECTPTR pInput)
{
    cQueue myQueue;
    BOOL bFound = FALSE;

    myQueue.Push((PVOID)pCurrent);

    while (!myQueue.IsEmpty() && !bFound)
    {
        
        pCurrent = (CONNECTPTR) myQueue.Pop();

        if (NULL != pCurrent->pLeft)
        {
            myQueue.Push((PVOID)pCurrent->pLeft);
        }
        if (NULL != pCurrent->pRight)
        {
            myQueue.Push((PVOID)pCurrent->pRight);
        }
        if (NULL == pCurrent->pLeft)
        {
            bFound = TRUE;
            pCurrent->pLeft = pInput;
        }
         else
         {
             if (NULL == pCurrent->pRight)
             {
                 bFound = TRUE;
                 pCurrent->pRight = pInput;
             }
         }

    }  // End of While
} // End of Descend
