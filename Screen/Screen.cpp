#include <stdio.h>
#include <windows.h>
#include "Screen.h"

cRectangle::cRectangle()                
{   
    m_rScreen.xStart = 0;
    m_rScreen.yStart = 0;
    m_rScreen.xEnd   = 100;
    m_rScreen.yEnd   = 100;
    
    m_rnHeadNode.rArea.xStart = 0;
    m_rnHeadNode.rArea.xEnd   = 100;
    m_rnHeadNode.rArea.yStart = 0;
    m_rnHeadNode.rArea.yEnd   = 100;
    
    m_rnHeadNode.pQuad[0] = NULL;
    m_rnHeadNode.pQuad[1] = NULL;
    m_rnHeadNode.pQuad[2] = NULL;
    m_rnHeadNode.pQuad[3] = NULL;
    
    m_rnHeadNode.pCurrRect = &m_rScreen;
}         

cRectangle::~cRectangle()                
{   
    // clean up used memory in our quad tree
}

BOOL cRectangle::AddRect(UINT uiX1, UINT uiY1, UINT uiX2, UINT uiY2)
{
    // do we care to make sure the points are passed in correctly 
    // check to make sure it fits within our area 
    if ((uiX1 < m_rScreen.xStart ) || (uiX2 < m_rScreen.xStart))
    {
        return FALSE;
    }         
    if ((uiX1 > m_rScreen.xEnd ) || (uiX2 > m_rScreen.xEnd))
    {
        return FALSE;
    }
    if ((uiY1 < m_rScreen.yStart ) || (uiY2 < m_rScreen.yStart))
    {
        return FALSE;
    }         
    if ((uiY1 > m_rScreen.yEnd ) || (uiY2 > m_rScreen.yEnd))
    {
        return FALSE;
    }
    
    // We need to add this rectangle into our list
    // does the rectangle passed in equal the size of the current rectangle?
    // YES - we are done.  Set the pointer and move on
    // NO - break up into 4 parts and pass to priv_addRect
    
    return TRUE;
    
}
HRESULT cRectangle::priv_AddRect(RNodePTR pCurrent, RECTANGLEPTR pNewRect)
{
    priv_CreateNewRect(pCurrent);
    return S_OK;
}
HRESULT cRectangle::priv_CreateNewRect(RNodePTR pCurrent)
{
    return S_OK;
}

HRESULT cRectangle::priv_InitNewRectNode(RNodePTR pCurrent)
{
    UINT uiCount;
    UINT uiList;
    // create 4 nodes
    // set each node's nodes to NULL
    // and set each node's RECTANGLEPTR
    for (uiCount =0; uiCount < 4; uiCount++)
    {
        pCurrent->pQuad[uiCount] = (RNodePTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RNODE));
        
        if (NULL == pCurrent->pQuad[uiCount])
        {
            // OK bad things happen clean up and leave
            for (uiList = 0; uiList < uiCount ; uiList++)
            {
                HeapFree(GetProcessHeap(),0, pCurrent->pQuad[uiList]); 
                pCurrent->pQuad[uiList] = NULL;
            }  // End of cleanup for loop
            return E_FAIL;
        }
        
        // set the rectangle to the current rectangle
        pCurrent->pQuad[uiCount]->pCurrRect = pCurrent->pCurrRect;
        for (uiList = 0; uiList < 4; uiList++)
        {
            pCurrent->pQuad[uiCount]->pQuad[uiList] = NULL;
        }  //  FOR loop to initialize the quadrants
        
    }  // end of the for loop for creation
    
    priv_InitQuad(pCurrent);
    
    // DONE creating the Quadrant nodes and initialzed them appropriately
    return S_OK;
}     

// Intialize the Quadrants in a rectangle
HRESULT cRectangle::priv_InitQuad(RNodePTR pCurrent)
{
    // Quadrant 0
    pCurrent->pQuad[0]->rArea.xStart= pCurrent->pCurrRect->xStart;
    pCurrent->pQuad[0]->rArea.yStart= pCurrent->pCurrRect->yStart;
    pCurrent->pQuad[0]->rArea.xEnd  = (pCurrent->pCurrRect->xStart + pCurrent->pCurrRect->xEnd) / 2;
    pCurrent->pQuad[0]->rArea.yEnd  = (pCurrent->pCurrRect->yStart + pCurrent->pCurrRect->yEnd) / 2;
    // Quadrant 1
    pCurrent->pQuad[0]->rArea.xStart= ((pCurrent->pCurrRect->xStart + pCurrent->pCurrRect->xEnd) / 2) + 1;
    pCurrent->pQuad[0]->rArea.yStart= pCurrent->pCurrRect->yStart;
    pCurrent->pQuad[0]->rArea.xEnd  = pCurrent->pCurrRect->xEnd;
    pCurrent->pQuad[0]->rArea.yEnd  = (pCurrent->pCurrRect->yStart + pCurrent->pCurrRect->yEnd) / 2;
    // Quadrant 2    
    pCurrent->pQuad[0]->rArea.xStart= pCurrent->pCurrRect->xStart;   
    pCurrent->pQuad[0]->rArea.yStart= ((pCurrent->pCurrRect->yStart + pCurrent->pCurrRect->yEnd) / 2) + 1;
    pCurrent->pQuad[0]->rArea.xEnd  = (pCurrent->pCurrRect->xStart + pCurrent->pCurrRect->xEnd) / 2; 
    pCurrent->pQuad[0]->rArea.yEnd  = pCurrent->pCurrRect->yEnd;   
    // Quadrant 3    
    pCurrent->pQuad[0]->rArea.xStart= ((pCurrent->pCurrRect->xStart + pCurrent->pCurrRect->xEnd) / 2) + 1;
    pCurrent->pQuad[0]->rArea.yStart= ((pCurrent->pCurrRect->yStart + pCurrent->pCurrRect->yEnd) / 2) + 1;
    pCurrent->pQuad[0]->rArea.xEnd  = pCurrent->pCurrRect->xEnd;   
    pCurrent->pQuad[0]->rArea.yEnd  = pCurrent->pCurrRect->yEnd;   
        
    return S_OK;
}

HRESULT cRectangle::FindRect(UINT uiX, UINT uiY, RECTANGLEPTR pRInput, UINT uiSize)
{
    RECTANGLEPTR prTemp;
    if ((NULL == pRInput) || (sizeof(RECTANGLE) != uiSize))
    {
        return E_FAIL;
    }
    
    //Check Size
    if ((uiX > m_rScreen.xEnd) || (uiY > m_rScreen.yEnd))
    {
        return E_FAIL;
    }
    
    prTemp = priv_FindRect(&m_rnHeadNode, uiX, uiY);
    
    return priv_CopyRect(pRInput, prTemp);
    
}

HRESULT cRectangle::priv_CopyRect(RECTANGLEPTR pDest, RECTANGLEPTR pSrc)
{
    if ((NULL != pDest) && (NULL != pSrc) && (pDest != pSrc))
    {
        // Copy the memory 
        memcpy(pDest, pSrc, sizeof(RECTANGLE));
        return S_OK;
    }
    else
    { 
        return E_FAIL;
    }
}

RECTANGLEPTR cRectangle::priv_FindRect(RNodePTR pCurrent, UINT uiX, UINT uiY)
{
    UINT uiXMid;
    UINT uiYMid;
    
    if ((NULL == pCurrent))
    {
        return NULL;
    }
    if ((NULL == pCurrent->pQuad[0]) && 
        (NULL == pCurrent->pQuad[1]) &&
        (NULL == pCurrent->pQuad[2]) &&
        (NULL == pCurrent->pQuad[3]) )
    {
        return pCurrent->pCurrRect;
    }
    uiXMid = (pCurrent->pCurrRect->xStart + pCurrent->pCurrRect->xEnd) /2;
    uiYMid = (pCurrent->pCurrRect->yStart + pCurrent->pCurrRect->yEnd) /2;
    
    if ( (pCurrent->pCurrRect->xStart <= uiX) && 
          (uiX < uiXMid) )
    {
        if ((pCurrent->pCurrRect->yStart <= uiY) &&
            (uiY < uiYMid) )
        {
            return priv_FindRect(pCurrent->pQuad[0], uiX, uiY);
        }
        else
        {
            return priv_FindRect(pCurrent->pQuad[1], uiX, uiY);
        }
    }
    else if ((pCurrent->pCurrRect->xEnd >= uiX) && // Quad 2
             (uiXMid <= uiX))
    {
        if ((pCurrent->pCurrRect->yStart <= uiY) &&
            (uiY < uiYMid) )
        {
            return priv_FindRect(pCurrent->pQuad[2], uiX, uiY);
        }
        else
        {
            return priv_FindRect(pCurrent->pQuad[3], uiX, uiY);
        }
    }
    else
    {
        printf("Something is wrong!\n");
        return NULL;
    }
}

RECTANGLEPTR cRectangle::priv_CreateRect(UINT uiX1, UINT uiY1, UINT uiX2, UINT uiY2)
{
    RECTANGLEPTR prTemp;
    
    prTemp = (RECTANGLEPTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RECTANGLE));
    if (NULL == prTemp)
    {
        return NULL;
    }
    
    return prTemp;
}
