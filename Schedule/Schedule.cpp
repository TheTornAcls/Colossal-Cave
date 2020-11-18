#include "schedule.h"

cSchedule::cSchedule()
{
    m_Head.pNext = NULL;
    iNumElements = 0;

    return;
}

cSchedule::~cSchedule()
{
    PNODE pTemp;

    while (m_Head.pNext != NULL)
    {
        pTemp = m_Head.pNext;
        m_Head.pNext = pTemp->pNext;
        LocalFree(pTemp);
    }

    return;
}

HRESULT cSchedule::Add2(INT iStart, INT iEnd, INT iStatus)
{
    PNODE pTemp;
    PNODE pTempPlus;

    PNODE pCurrent;
    PNODE pPrev;

    pTemp = CreateNode(iStart, iEnd,iStatus);
    // create an element and initialize with data
    if (NULL == pTemp)
    {
        // Node Creation failed
        return E_FAIL;
    }

    pPrev = &m_Head;
    pCurrent = pPrev->pNext;
    // walk through the current orgainzed list of items
    while (NULL != pCurrent)
    {
        pTempPlus = NULL;

        // Does our Temp element start before our current element starts
        if (pTemp->iStart < pCurrent->iStart)
        {
            // Does our Temp element end after our current element starts (We have overlap)
            if (pTemp->iEnd > pCurrent->iStart)
            {
                pTempPlus = CreateNode(pCurrent->iStart,pTemp->iEnd, pTemp->iStatus);
                pTemp->iEnd = pCurrent->iStart;

            }
            pPrev->pNext = pTemp;
            pTemp->pNext = pCurrent;
            iNumElements++;
        }

        else if (pTemp->iEnd <=  pCurrent->iEnd)
        {
                // OK here we have our Temp element completely sitting within the Current elment
                // free the element and return
                LocalFree(pTemp);
                return S_OK;
        }
        else if (pTemp->iStart < pCurrent->iEnd)
        {

            // Ok temp element starts before current element ends, but ends after the Current element
            pTemp->iStart = pCurrent->iEnd;
        }



        if (NULL == pTempPlus)
        {
            if (pPrev->pNext == pTemp)
            {
                // OK we added the element
                return S_OK;
            }
            // Nope we are still searching
            pPrev = pCurrent;
            pCurrent = pCurrent->pNext;
        }
        else
        {
           pTemp = pTempPlus;
        }
    }

    if (NULL == pCurrent)
    {
        // add the element to the end of thelist
        iNumElements++;
        pPrev->pNext = pTemp;
    }

    return S_OK;
}

HRESULT cSchedule::Add(INT iStart, INT iEnd, INT iStatus)
{
    PNODE pTemp;
    PNODE pTempPlus;

    PNODE pCurrent;
    PNODE pPrev;

    pTemp = CreateNode(iStart, iEnd,iStatus);
    // create an element and initialize with data
    if (NULL == pTemp)
    {
        // Node Creation failed
        return E_FAIL;
    }

    pPrev = &m_Head;
    pCurrent = pPrev->pNext;
    // walk through the current orgainzed list of items
    while (NULL != pCurrent)
    {
        pTempPlus = NULL;

        // Does our Temp element start before our current element starts
        if (pTemp->iStart < pCurrent->iStart)
        {
            // Does our Temp element end after our current element starts (We have overlap)
            if (pTemp->iEnd > pCurrent->iStart)
            {
                //Do we have the same status
                if (pCurrent->iStatus != pTemp->iStatus)
                {
                    pTempPlus = CreateNode(pCurrent->iStart,pTemp->iEnd, pTemp->iStatus);
                    pTemp->iEnd = pCurrent->iStart;
                }
                else
                {   
                    pTempPlus = pCurrent;
                    INT iTempStart = pCurrent->iStart;
                    pCurrent->iStart = pTemp->iStart;
                    pTemp->iStart = iTempStart;
                  
                }

            }
            if (pTempPlus != pCurrent)
            {
                pPrev->pNext = pTemp;
                pTemp->pNext = pCurrent;
                iNumElements++;
            }
            else
            {
                DumpNode(pCurrent);
                DumpNode(pTemp);
            }
        }

        else if (pTemp->iEnd <=  pCurrent->iEnd)
        {
                // OK here we have our Temp element completely sitting within the Current elment
                // free the element and return
                LocalFree(pTemp);
                return S_OK;
        }
        else if (pTemp->iStart < pCurrent->iEnd)
        {

            // Ok temp element starts before current element ends, but ends after the Current element
            pTemp->iStart = pCurrent->iEnd;
        }



        if (NULL == pTempPlus)
        {
            if (pPrev->pNext == pTemp)
            {
                // OK we added the element
                return S_OK;
            }
            // Nope we are still searching
            pPrev = pCurrent;
            pCurrent = pCurrent->pNext;
        }
        else if (pCurrent == pTempPlus)
        {
            pTempPlus = NULL;
        }
        else
        {
           pTemp = pTempPlus;
        }
    }

    if (NULL == pCurrent)
    {
        // add the element to the end of thelist
        iNumElements++;
        pPrev->pNext = pTemp;
    }

    return S_OK;
}

// Give a start/End/Status create and return an initialized PNODE object
PNODE cSchedule::CreateNode(INT iStart, INT iEnd, INT iStatus)
{
    PNODE pTemp;

    pTemp = (PNODE)LocalAlloc(LPTR, sizeof(NODE));
    if (NULL != pTemp)
    {
        // Node Creation Succeeded
        // Save the data
        pTemp->iStart = iStart;
        pTemp->iEnd   = iEnd;
        pTemp->iStatus = iStatus;
        pTemp->pNext   = NULL;
    }

    return pTemp;

}

void cSchedule::Print()
{
    PNODE pTemp = m_Head.pNext;
    INT iCurrent = 1;

    // Go through the list and print out the list
    printf("1     2     3     4     5     6\n");

    while (NULL != pTemp) 
    {
        while(iCurrent != pTemp->iStart)
        {
            iCurrent++;
            printf("      ");

        }
        while (iCurrent != pTemp->iEnd)
        {
            switch (pTemp->iStatus)
            {
            case BUSY:
                printf("******");
                break;
            case AWAY:
                printf("xxxxxx");
                break;
            default:
                printf("      ");
            }
            iCurrent++;
        }

        pTemp = pTemp->pNext;
    }
    printf("\n");

    return;
}

HRESULT cSchedule::Input(PTNODE ptList, INT iNumber)
{
    INT iStatus;
    INT iList;

    // Set the Status you want to find in your list
    for (iStatus = 0; iStatus <= FREE; iStatus++)
    {
       iList = 0;

       //Walk through the list looking for a particular status
       while (iList < iNumber)
       {
           if (ptList[iList].iStatus == iStatus)
           {
               Add(ptList[iList].iStart,ptList[iList].iEnd, ptList[iList].iStatus);
           }
           iList++;
       }
    }

    return S_OK;
}

HRESULT cSchedule::ReturnSummary(PTNODE pTNODE,PINT piSize)
{
    if (NULL == pTNODE)
    {
        // OK they just want the size of the needed object
        printf("cSchedule::ReturnSummary iNumElements = %d\n",iNumElements);
        *piSize = sizeof(TNODE) * iNumElements;
    }
    else
    {
        *piSize = 0;
    }

    return S_OK;
}

void cSchedule::DumpNode(PNODE pInput)
{
    printf("\nPNODE = 0x%x\n",pInput);
    printf("iStart=%d\n",pInput->iStart);
    printf("iEnd=%d\n",pInput->iEnd);
    printf("iStatus=%d\n\n",pInput->iStatus);
}
