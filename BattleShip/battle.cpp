#include <stdio.h>
#include <windows.h>

#include "battle.h"

cBoard::cBoard()
{
    INT iX, iY;

    // initialize the board to all NULLs
    for (iX =0; iX <BOARDSIZE; iX++)
    {
        for (iY = 0; iY < BOARDSIZE; iY++)
        {
            Board[iX][iY] = NULL;
        }
    }
}

cBoard::~cBoard()
{

}

BOOL cBoard::Add(INT iShip, INT iX[], INT iY[])
{

    if (0 == iShip)
    {
        // no elements in the array
        return FALSE;
    }
    

    return TRUE;
}

BYTE cBoard::Fire(INT iX, INT iY)
{
    // Check to make sure coordinates are legit
    if ((iX > BOARDSIZE) || (iX < 1) || (iY < 1) || (iY > BOARDSIZE))
    {
        printf("Invalid coordinates\n");
        return INVALID;
    }

    // We will adjust for the different counting schemes.
    iX--;
    iY--;

    if (NULL == Board[iX][iY])
    {
        printf("You Missed\n");
        return MISS;
    }

    Board[iX][iY]->iHits--;

    if (0 == Board[iX][iY]->iHits)
    {
        Board[iX][iY] = NULL;
        printf("Sunk\n");
        return SUNK;
    }
    else
    {
        printf("Hit\n");
        return HIT;
    }
}
