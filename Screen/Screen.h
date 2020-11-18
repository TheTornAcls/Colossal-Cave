#ifndef _SCREEN
#define _SCREEN

// Quadrants 
// -------> X
// | 0 | 1
// | --+--
// | 2 | 3
// Y
typedef struct _RECTANGLE *RECTANGLEPTR;
typedef struct _RECTANGLE
{
    UINT xStart;
    UINT yStart;
    UINT xEnd;
    UINT yEnd;
} RECTANGLE;

typedef struct _RNODE *RNodePTR;

typedef struct _RNODE
{
    RECTANGLEPTR pCurrRect;
    RECTANGLE    rArea;
    RNodePTR pQuad[4];
} RNODE;


class cRectangle
{
public:
    cRectangle();
    ~cRectangle();
    BOOL AddRect(UINT uiX1, UINT uiY1, UINT uiX2, UINT uiY2);
    HRESULT FindRect(UINT uiX, UINT uiY, RECTANGLEPTR pRInput, UINT uiSize );
    
private:
     HRESULT priv_CopyRect(RECTANGLEPTR pDest, RECTANGLEPTR pSrc);
     RECTANGLEPTR priv_FindRect(RNodePTR pCurrent, UINT uiX, UINT uiY);
     RECTANGLEPTR priv_CreateRect(UINT uiX1, UINT uiY1, UINT uiX2, UINT uiY2);
     HRESULT priv_AddRect(RNodePTR pCurrent, RECTANGLEPTR pNewRect);
     HRESULT priv_InitQuad(RNodePTR pCurrent);
     HRESULT priv_CreateNewRect(RNodePTR pCurrent);
     HRESULT priv_InitNewRectNode(RNodePTR pCurrent)
     
     RECTANGLE  m_rScreen;
     RNODE      m_rnHeadNode;
    
    
};

#endif  // _SCREEN
