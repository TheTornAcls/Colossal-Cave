#include "stdafx.h"

cBoard::cBoard()
{
    int x, y;
    // initialize Next Healthy Indexes
    nhX = -1;
    nhY = 0;

    m_CurrentQueen = 0;

    // Need to intialize board to all zeros
    for (y = 0; y < 8; y ++)
    {
        for (x = 0; x < 8; x++)
        {
            m_Board[x][y] = 0;
        }
    }

}

cBoard::~cBoard()
{
}

BOOL cBoard::NextHealthy(int *x, int *y)
{

    INT iX;
    INT iY;

    if (nhY >= 8)
    {
        return FALSE;
    }

    iX = nhX;
    iY = nhY;

    while (TRUE)
    {
        iX++;

        // do wrap around logic
        if (iX >= 8)
        {
            iX = 0 ;
            iY++;
            if (iY >= 8)
            {
                // NOTE Next Healthy is in an UNKNOWN State!!
                return FALSE;
            }
        }

        if (!m_Board[iX][iY])
        {
            *x = iX;
            *y = iY;
            return TRUE;
        }

    } // END WHILE

    return TRUE;
}


INT cBoard::ReturnTotalThreat()
{
    int iTotalThreat = 0;
    int y;

    for (y = 0; y < 8; y++)
    {
        iTotalThreat +=  m_Board[m_Queens[y]][y];
    }

    return iTotalThreat;
}

void cBoard::PrintBoard()
{
    INT x, y;

    for (y = 0; y< 8; y++)  // Down
    {
        for (x = 0; x < 8; x++) // Across
        {
            if ( x == m_Queens[y])
            {
                printf("Q\t");
            }
            else
            {
                printf("*\t");
            }
        }
        printf("\n");
    }
}

void cBoard::priv_NegativeDiagonal(int x, int y, int i)
{
    int startX, startY, b;

    // find the slope
    b = y + x;

    if (b >= 8)
    {
        startY = 7;
        startX = b - startY;
    }
    else
    {
        startX = 0;
        startY = b;
    }

    while ((startX < 8) && (startY >= 0))
    {
        if ((startX != x) &&
            (startY != y))
        {
            m_Board[startX][startY]+= i;
        }
        startX++;
        startY--;
    }

    return;
}

void cBoard::priv_PositiveDiagonal(int x, int y, int i)
{
    int b;
    int startX, startY;

    // find slope
    b = y - x;

    if (b < 0)
    {
        startY = 0;
        startX = 0 - b;
    }
    else
    {
        startX = 0;
        startY = b;
    }

    while ((startX < 8) && (startY < 8))
    {
        if ((startX != x) &&
            (startY != y))
        {
            m_Board[startX][startY] += i;
        }
        startX++;
        startY++;
    }

    return;
}

void cBoard::priv_Horizontal(int x, int y,int i)
{
    int xCount;

    for (xCount = 0; xCount < 8; xCount++)
    {
        if (x !=  m_Queens[y])
        {
            m_Board[xCount][y] += i;
        }
    }

    return;
}

void cBoard::priv_Vertical(int x, int y, int i)
{
    int yCount;


    for (yCount = 0; yCount < 8; yCount++)
    {
        if (y != yCount)
        {
            m_Board[x][yCount]+= i;
        }
    }

    return;
}
void cBoard::Take(int x, int y)
{
    nhX = x;
    nhY = y;

    Poison(x,y);
}

void cBoard::Poison(int x, int y)
{

        // Vertical Loop
        priv_Vertical(x,y,1);

        // Horizontal Loop
        priv_Horizontal(x,y,1);

        // Negative Diagonal
        priv_NegativeDiagonal(x,y,1);

        // Right Diagonal
        priv_PositiveDiagonal(x,y,1);

    return;
}
VOID cBoard::Heal(int x, int y)
{
    // Vertical Loop
    priv_Vertical(x,y,-1);

    // Horizontal Loop
    priv_Horizontal(x,y,-1);

    // Negative Diagonal
    priv_NegativeDiagonal(x,y,-1);

    // Right Diagonal
    priv_PositiveDiagonal(x,y,-1);
    return;
}
void cBoard::PrintThreat()
{
    INT x, y;

    for (y = 0; y< 8; y++)  // Down
    {
        for (x = 0; x < 8; x++) // Across
        {      
            printf("%d\t",m_Board[x][y]);
        }
        printf("\n");
    }
}

{
    if (!NextHealthy(&x, &y))
    {
        // if there is no healthy one
        // Reset the Current Queen back 1
        // need to heal the previous Piece (now the current queen)
        // Reset nhX and nhY to the current piece's position
        // AND then find the next healthy

        // to determine mirroring
        // Just need to keep the list of pieces around
        // data structure
        // Count - number of times found
        // Queens - an array of where the pieces are
        // might be able to speed the search of mirror pieces by using a hash table
    }
    else 
    {
        // take the current piece
        // Poison the correct board squares
        // assign the NextHealthy values to the current Queen
        // m_CurrentQueen++
    }
    // how do you know when to terminate the loop??
    // Add a method to inform you of what the current Queen is
    // Or just a method with TRUE or FALSE (it will check the value of
    // the queen and return the value
}

// Can you improve finding the next healthy??
// start on the row of the current piece to look
// don't complete searching the row where you know you just took
// a piece
