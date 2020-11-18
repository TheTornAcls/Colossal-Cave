#include <windows.h>
#include <stdio.h>

#include "pop.h"
#include "binary.h"
#include "connect.h"
        
cBinary::cBinary()
{
    //printf("cBinary Contructor\n");
    m_pHead = NULL;
}
cBinary::~cBinary()
{
    TreeDelete(m_pHead);
    //printf("cBinary DeContructor\n");
}

CONNECTPTR cBinary::NodeCreate(CHAR cChar)
{
    CONNECTPTR pTemp;
    pTemp = (CONNECTPTR)LocalAlloc(LPTR, sizeof(CONNECT));

    return pTemp;
}

void cBinary::TreeDelete(CONNECTPTR pCurrent)
{
    if (NULL != pCurrent)
    {
        TreeDelete(pCurrent->pLeft);
        TreeDelete(pCurrent->pRight);
        LocalFree(pCurrent);
    }
    return;
}

void cBinary::AddItem(CHAR cChar)
{
    CONNECTPTR pTemp;

    pTemp = NodeCreate(cChar);

    if (NULL != pTemp)
    {
        pTemp->pLeft = NULL;
        pTemp->pRight = NULL;
        pTemp->cData = cChar;
        if (m_pHead == NULL)
        {
            m_pHead = pTemp;
        }
        else
        {
            Descend(m_pHead, pTemp);
        }

    }

}
void cBinary::Descend(CONNECTPTR pCurrent, CONNECTPTR pTemp)
{
    //printf("cBinary::Descend\n");
    if (pCurrent->cData >= pTemp->cData)
    {
        if (NULL == pCurrent->pLeft)
        {
            pCurrent->pLeft = pTemp;
        }
        else
        {
            Descend(pCurrent->pLeft, pTemp);
        }
    }
    else
    {
        if (NULL == pCurrent->pRight)
        {
            pCurrent->pRight = pTemp;
        }
        else
        {
            Descend(pCurrent->pRight, pTemp);
        }
    }
    return;
}       

void cBinary::PrintLevel()
{
    cQueue myQueue;
    CONNECTPTR pTemp;

    myQueue.Push((PVOID)m_pHead);

    while (!myQueue.IsEmpty())
    {
        
        pTemp = (CONNECTPTR) myQueue.Pop();

        printf("%c ", pTemp->cData);
        if (NULL != pTemp->pLeft)
        {
            myQueue.Push((PVOID)pTemp->pLeft);
        }
        if (NULL != pTemp->pRight)
        {
            myQueue.Push((PVOID)pTemp->pRight);
        }

    }
    printf("\n");
}


void cBinary::PrintChildren()
{
    cQueue myQueue;
    CONNECTPTR pTemp;

    Identification();
    printf("PrintChildern - Recursive\n");
    FindChildren(&myQueue,m_pHead);
      
    while (!myQueue.IsEmpty())
    {
        pTemp = (CONNECTPTR)myQueue.Pop();
        printf("%c ", pTemp->cData);
    }
    printf("\n");

    Identification();
    printf("PrintChildern - Stack\n");
    FindChildrenStack(&myQueue,m_pHead);
      
    while (!myQueue.IsEmpty())
    {
        pTemp = (CONNECTPTR)myQueue.Pop();
        printf("%c ", pTemp->cData);
    }
    printf("\n");
}

void cBinary::Identification()
{
    printf("cBinary::");
}

void cBinary::FindChildren(cStructure * pQueue, CONNECTPTR pInput)
{
    if ( (NULL == pInput->pLeft) && (NULL == pInput->pRight))
    {
        // found a chld
        pQueue->Push((PVOID)pInput);

    }
    else
    {
        if (NULL != pInput->pLeft)
        {
            FindChildren(pQueue,pInput->pLeft);
        }
        if (NULL != pInput->pRight)
        {
            FindChildren(pQueue,pInput->pRight);
        }
    }
}
void cBinary::FindChildrenStack(cStructure * pQueue, CONNECTPTR pInput)
{
    cStack myStack;

    myStack.Push((PVOID)pInput);

    while (!myStack.IsEmpty())
    {
        pInput = (CONNECTPTR)myStack.Pop();
        if ( (NULL == pInput->pLeft) && (NULL == pInput->pRight))
        {
            // found a chld
            pQueue->Push((PVOID)pInput);

        }
        else
        {
            if (NULL != pInput->pRight)
            {
                //FindChildren(pQueue,pInput->pRight);
                myStack.Push((PVOID)pInput->pRight);
            }
            if (NULL != pInput->pLeft)
            {
                //FindChildren(pQueue,pInput->pLeft);
                myStack.Push((PVOID)pInput->pLeft);
            }
        }
    }
}

bool cBinary::ValidateTree()
{
	return PrivateValidateTree(m_pHead);
}

bool cBinary::PrivateValidateTree(CONNECTPTR currentNode)
{
	return false;
}
