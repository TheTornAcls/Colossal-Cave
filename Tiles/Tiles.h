#ifndef _TILES_H
#define _TILES_H

class cBoard
{
public:
    cBoard();
    ~cBoard();

    void Push(BYTE bNumber);
    BOOL CheckTile();
    BYTE Pop();
    void PrintOut();

private:
    BYTE m_bStack[17];
    BYTE m_bStackTop;

    BYTE m_bAcross;
    BYTE m_bDown;

    cQueue m_QueueTile;
    cTiles * m_TileTable[MAX_ACROSS][MAX_DOWN];

// We need an array of Tiles
    cTiles m_cTileList[MAX_ACROSS*MAX_DOWN];

    UINT m_uiNumOfPops;
    UINT m_uiNumOfPushes;
    UINT m_uiNumOfRotations;

};

#endif
