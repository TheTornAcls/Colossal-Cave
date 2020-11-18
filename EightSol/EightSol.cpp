// EightSol.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int iBoard[8][8];
LOCATION Queens[8];  

LOCATION PrevPiece;

INT lastY;

void ClearBoard()
{
    int x,y;
    for (y=0; y<8; y++)
    {
        for (x=0; x<8; x++)
        {
                iBoard[x][y] = 0;
        }
    }
}

void InitializeBoards()
{
    int y;

    ClearBoard();

    for (y=0; y<8 ; y++)
    {
        Queens[y].x = 0;
        Queens[y].y = y;
    }

    PrevPiece.x = 8;
    PrevPiece.y = 8;

    lastY = 8;

    return;
}

void MyInitializeBoardsGood()
{

    ClearBoard();

        Queens[0].x = 3;
        Queens[0].y = 0;

        Queens[1].x = 1;
        Queens[1].y = 1;

        Queens[2].x = 7;
        Queens[2].y = 2;

        Queens[3].x = 5;
        Queens[3].y = 3;

        Queens[4].x = 0;
        Queens[4].y = 4;

        Queens[5].x = 2;
        Queens[5].y = 5;

        Queens[6].x = 4;
        Queens[6].y = 6;

        Queens[7].x = 6;
        Queens[7].y = 7;

    return;
}

int ReturnTotalThreat()
{
    int iTotalThreat = 0;
    int iCount;

    for (iCount = 0; iCount < 8; iCount++)
    {
        iTotalThreat +=  iBoard[Queens[iCount].x][Queens[iCount].y];
    }

    return iTotalThreat;
}

void PrintBoard()
{
    INT x, y;

    for (y = 0; y< 8; y++)  // Down
    {
        for (x = 0; x < 8; x++) // Across
        {
            if ( (y == Queens[y].y) && (x == Queens[y].x))
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

void PrintThreat()
{
    INT x, y;

    for (y = 0; y< 8; y++)  // Down
    {
        for (x = 0; x < 8; x++) // Across
        {      
            printf("%d\t",iBoard[x][y]);
        }
        printf("\n");
    }
}

void PrintQueenThreat()
{
    int y;

    printf("\n");
    for (y = 0; y < 8; y++)
    {
        printf("%d,%d\t", Queens[y].x,Queens[y].y);
    }
    printf("\n");
    for (y = 0; y < 8; y++)
    {
        printf("%d\t", iBoard[Queens[y].x][Queens[y].y]);
    }
    printf("\n");
}

void NegativeDiagonal(int iPiece)
{
    int x, y, b;

    x = Queens[iPiece].x;
    y = Queens[iPiece].y;

    b = y + x;

    if (b >= 8)
    {
        y = 7;
        x = b - y;
    }
    else
    {
        x = 0;
        y = b;
    }

    while ((x < 8) && (y >= 0))
    {
        if ((x != Queens[iPiece].x) &&
            (y != Queens[iPiece].y))
        {
            iBoard[x][y]++;
        }
        x++;
        y--;
    }

    return;
}

void PositiveDiagonal(int iPiece)
{
    int x, y, b;

    x = Queens[iPiece].x;
    y = Queens[iPiece].y;

    b = y - x;

    if (b < 0)
    {
        y = 0;
        x = 0 - b;
    }
    else
    {
        x = 0;
        y = b;
    }

    while ((x < 8) && (y < 8))
    {
        if ((x != Queens[iPiece].x) &&
            (y != Queens[iPiece].y))
        {
            iBoard[x][y]++;
        }
        x++;
        y++;
    }

    return;
}

void Horizontal(int iPiece)
{
    int x, y;

    y = Queens[iPiece].y;


    for (x = 0; x < 8; x++)
    {
        if (x != Queens[iPiece].x)
        {
            iBoard[x][y]++;
        }
    }

    return;
}

void Vertical(int iPiece)
{
    int x, y;

    x = Queens[iPiece].x;


    for (y = 0; y < 8; y++)
    {
        if (y != Queens[iPiece].y)
        {
            iBoard[x][y]++;
        }
    }

    return;
}

void ComputeThreat()
{
    INT iCount;

    ClearBoard();


    for (iCount = 0; iCount < 8; iCount++)      // go through the pieces
    {
        // Vertical Loop
        Vertical(iCount);

        // Horizontal Loop
        Horizontal(iCount);

        // Negative Diagonal
        NegativeDiagonal(iCount);

        // Right Diagonal
        PositiveDiagonal(iCount);

    }
    return;
}

void FindLeastThreat()
{
    INT x,y;
    INT maxThreat = 0;
    INT minThreat;
    BOOL Found = FALSE;
    INT minX;

    // Find Queen with the greatest threat level
    for (y = 0; y < 8; y++)
    {
        if (y != lastY)
        {
            if (maxThreat < iBoard[Queens[y].x][Queens[y].y])
            {
                maxThreat = iBoard[Queens[y].x][Queens[y].y];
            }
        }
    }

    if (maxThreat != 0)
    {             
        y = 0;
        while ( (y < 8) && !Found)
        {
            if (y != lastY)
            {
                if (maxThreat = iBoard[Queens[y].x][Queens[y].y])
                {
                    Found = TRUE;
                    lastY = y;
                }
            }
            y++;

        }
        minThreat = 9;

        // move that queen to the space in the same row with the SMALLEST threat
        for (x = 0; x < 8; x++)
        {
            if (minThreat > iBoard[x][lastY])
            {
                // make sure we are moving the same piece to the same location
                if (x != Queens[lastY].x)
                {
                    minThreat = iBoard[x][lastY];
                    minX = x;
                }
            }
        }  // end of FOR x

        // Move Queen
        if (minThreat < 8)  
        {
            if (minX > 7)
            {
                printf("Error\n");
            }
            Queens[lastY].x = minX;
            Queens[lastY].y = lastY;
        }


    }


    return;
}

int main(int argc, char * argv[])
{
    int numTurns = 0;
    int numRequest = 100;

    int time;


    if (argc > 1)
    {
        numRequest = atoi((char *) argv[1]);
        printf("2nd argument %d\n", numRequest);
    }

    time = GetTickCount();

    InitializeBoards();
    
    ReturnTotalThreat();

    ComputeThreat();
    while (( 0 < ReturnTotalThreat()) && (numTurns < numRequest))
    {
        FindLeastThreat();
        ComputeThreat();
    
        numTurns++;
    }


    PrintBoard();

    PrintThreat();
    PrintQueenThreat();

    printf("Total Time = %d\n in %d turns", GetTickCount() - time, numTurns);

	return 0;
}

