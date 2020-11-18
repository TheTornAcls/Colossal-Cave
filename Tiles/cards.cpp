
#include "stdafx.h"

cTiles::cTiles()
{
    m_uiTotalRotations = 0;
    m_dwUp = 0;
    return;
}

cTiles::~cTiles()
{
    return;
}

// Rotates the tile Clockwise
HRESULT cTiles::Rotate()
{
    m_dwUp = ++m_dwUp % 4;
    m_uiTotalRotations++;

    return S_OK;
}
              
// Get the Up element
DWORD cTiles::get_Up()
{
    return m_dwSides[m_dwUp];
}

// Get the Right Element
DWORD cTiles::Get_Right()
{
    return m_dwSides[(m_dwUp + 1) % 4];
}

// Get the Down Element
DWORD cTiles::Get_Down()
{
    return m_dwSides[(m_dwUp + 2) % 4];
}

// Get the Left Element
DWORD cTiles::Get_Left()
{
    return m_dwSides[(m_dwUp + 3) % 4];
}


HRESULT cTiles::Initialize(BYTE bUp, BYTE bRight, BYTE bDown, BYTE bLeft)
{
    m_dwSides[0] = bUp;
    m_dwSides[1] = bRight;
    m_dwSides[2] = bDown;
    m_dwSides[3] = bLeft;

    return S_OK;
}
