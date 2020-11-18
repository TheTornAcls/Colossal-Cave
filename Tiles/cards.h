#ifndef _CARDS_H
#define _CARDS_H

#define MAX_ACROSS 5
#define MAX_DOWN   5

#define ORANGE 0
#define GREEN  1
#define PURPLE 2
#define EARTH  3
#define SATURN 4
#define MOON   5
#define IO     6
#define MARS   7

class cTiles{
public:
    cTiles();
    ~cTiles();

    // Rotates the tile Clockwise
    STDMETHODIMP Rotate();

    // Retrieve the different elements 
    STDMETHODIMP_(DWORD) get_Up();
    STDMETHODIMP_(DWORD) Get_Right();
    STDMETHODIMP_(DWORD) Get_Down();
    STDMETHODIMP_(DWORD) Get_Left();

    STDMETHODIMP Initialize(BYTE bUp, BYTE bRight, BYTE bDown, BYTE bLeft);
    UINT m_uiTotalRotations;
private:
    DWORD m_dwSides[4];

    DWORD m_dwUp;
};

#endif
