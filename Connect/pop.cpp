#include <windows.h>
#include <stdio.h>

#include "pop.h"
#include "binary.h"
#include "connect.h"

cStructure::cStructure()
{
    m_pHead = NULL;
    m_pTail = NULL;
}

cStructure::~cStructure()
{
    NODEPTR pTemp;

    // Destroy the linked List
    while (NULL != m_pHead)
    {
        pTemp = m_pHead;
        m_pHead = m_pHead->pNext;
        LocalFree(pTemp);
    }
}

BOOL cStructure::IsEmpty()
{
    if (NULL == m_pHead)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

PVOID cStructure::Pop()
{

    if (NULL == m_pHead)
    {
        return NULL;
    }
    else
    {
        NODEPTR pTemp = m_pHead;
        PVOID pRet = pTemp->pLeaf;

        m_pHead = pTemp->pNext;

        LocalFree(pTemp);

        return pRet;
        
    }
}
//**********************************************************
// cQueue
//**********************************************************
cQueue::cQueue()
{
}

cQueue::~cQueue()
{
}

void cQueue::Push(PVOID pInput)
{
    NODEPTR pTemp;
    // Create the node
    
    pTemp = (NODEPTR) LocalAlloc(LPTR, sizeof(NODE));
    if (NULL != pTemp)
    {
        pTemp->pNext = NULL;
        pTemp->pLeaf = pInput;
        if (NULL == m_pHead)
        {
            m_pHead = pTemp;
            m_pTail = pTemp;
        }
        else
        {
            m_pTail->pNext = pTemp;
            m_pTail = pTemp;
        }
    }

    return;
}

//**********************************************************
// cStack
//**********************************************************

cStack::cStack()
{
}

cStack::~cStack()
{
}

void cStack::Push(PVOID pInput)
{
    NODEPTR pTemp;
    // Create the node
    
    pTemp = (NODEPTR) LocalAlloc(LPTR, sizeof(NODE));
    if (NULL != pTemp)
    {
        pTemp->pLeaf = pInput;

        if (NULL == m_pHead)
        {
            m_pHead = pTemp;
            m_pTail = pTemp;
            pTemp->pNext = NULL;
        }
        else
        {          
            pTemp->pNext = m_pHead;
            m_pHead = pTemp;
        }
    }

    return;
}
