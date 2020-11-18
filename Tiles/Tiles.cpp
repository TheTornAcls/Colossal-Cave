// Tiles.cpp : Defines the entry point for the console application.

#include "stdafx.h"

void cBoard::Push(BYTE bNumber)
{
    // increment stack head
    m_bStackTop++;

    m_bStack[m_bStackTop] = bNumber;

    // OK we have added something to the stack increment the Tile pointers
    m_bAcross++;
    if (m_bAcross >= MAX_ACROSS)
    {
        m_bAcross = 1;
        m_bDown++;
    }

    m_uiNumOfPushes++;

    return;
}

BYTE cBoard::Pop()
{
    if (m_bStackTop)
    {
        BYTE bReturn;

        m_uiNumOfPops++;

        bReturn = m_bStack[m_bStackTop];

        m_bStackTop--;

        // OK we have removed something to the stack decrement the Tile pointers
        m_bAcross--;
        if (m_bAcross < 1)
        {
            m_bAcross = MAX_ACROSS -1;
            m_bDown--;
        }

        return bReturn;
    }
    return 17;
}

BOOL cBoard::CheckTile()
{
    BOOL bDone;
    BYTE bNumberLeft = m_QueueTile.SizeOfQ();
    BYTE bQPops = 0;
    BYTE bCurrent;
    BYTE bRotations;

    // check to see if we have anything left in the Q
    if (bNumberLeft == 0)
    {
        bDone = TRUE;
    } else
    {
        bDone = FALSE;
    }

    while (!bDone && (bQPops < bNumberLeft ))
    {
        bRotations = 0;
        bCurrent = m_QueueTile.Remove();

        bQPops++;

        while (!bDone && (bRotations < 4) )
        {
            // does the card fit
            if ( (m_TileTable[m_bAcross -1][m_bDown] == NULL && m_TileTable[m_bAcross][m_bDown-1] == NULL) ||
                 (m_TileTable[m_bAcross - 1][m_bDown] == NULL && ( m_TileTable[m_bAcross][m_bDown - 1]->Get_Down() == m_cTileList[bCurrent].get_Up())) ||
                 (m_TileTable[m_bAcross][m_bDown - 1] == NULL && ( m_TileTable[m_bAcross - 1][m_bDown]->Get_Right() == m_cTileList[bCurrent].Get_Left())) ||
                 ( (m_TileTable[m_bAcross -1][m_bDown] != NULL && m_TileTable[m_bAcross][m_bDown-1] != NULL) &&
                   ( ( m_TileTable[m_bAcross][m_bDown - 1]->Get_Down() == m_cTileList[bCurrent].get_Up() ) && ( m_TileTable[m_bAcross - 1][m_bDown]->Get_Right() == m_cTileList[bCurrent].Get_Left()))) )
            {
                m_TileTable[m_bAcross][m_bDown] = &m_cTileList[bCurrent];
                Push(bCurrent);

                bDone = CheckTile();
                if (!bDone)
                {
                    // card fit but couldn't find match
                    Pop();
                }
            }

            if (!bDone)
            {
                m_cTileList[bCurrent].Rotate();
                bRotations++;
                m_uiNumOfRotations++;
            }
        }

        if (!bDone)
        {
            // Rotate the tile and try again
            m_QueueTile.Add(bCurrent);
        }
    }


    return bDone;
}

cBoard::cBoard()
{
    m_bAcross = 1;
    m_bDown = 1;

    m_uiNumOfPops = 0;
    m_uiNumOfPushes= 0;;
    m_uiNumOfRotations = 0;

    INT iCount;
    INT iCountDown;


    // Initialize the stack head
    m_bStackTop = 0;

    // Initial the table to cTiles to NULL
    for (iCount = 0; iCount < MAX_ACROSS; iCount++)
    {
        for (iCountDown = 0; iCountDown < MAX_DOWN;iCountDown++)
        {
            m_TileTable[iCount][iCountDown] = NULL;
        }
    }

    // We need to initialze the cards

    // Initialize each tile with the correct symbols
    m_cTileList[0].Initialize(ORANGE, GREEN, PURPLE, GREEN);  // Found

    m_cTileList[1].Initialize(ORANGE, PURPLE, GREEN, ORANGE);  // Found
    m_cTileList[2].Initialize(ORANGE, PURPLE, GREEN, ORANGE);  // Found
    m_cTileList[3].Initialize(ORANGE, PURPLE, GREEN, EARTH);   // Found
    m_cTileList[4].Initialize(EARTH, SATURN, SATURN, MOON);     // Found
    m_cTileList[5].Initialize(GREEN, PURPLE, GREEN, ORANGE);    // Found
    m_cTileList[6].Initialize(EARTH, IO, ORANGE, GREEN);        // Found
    m_cTileList[7].Initialize(MARS, GREEN, PURPLE, PURPLE);     // Found
    m_cTileList[8].Initialize(EARTH, PURPLE, MARS, GREEN);     // Found
    m_cTileList[9].Initialize(MOON, MARS, MARS, IO);           // Found
    m_cTileList[10].Initialize(GREEN, IO, GREEN, PURPLE);      // Found
    m_cTileList[11].Initialize(GREEN, PURPLE, MARS, SATURN);  // Found
    m_cTileList[12].Initialize(MARS, SATURN, GREEN, PURPLE);   // Found
    m_cTileList[13].Initialize(GREEN, PURPLE, MARS, ORANGE);  // Found
    m_cTileList[14].Initialize(ORANGE, ORANGE, IO, EARTH);     // Found
    m_cTileList[15].Initialize(PURPLE, GREEN, ORANGE, PURPLE);  // Found

    // Add each tile to the QUEUE
    for (iCount = 15; iCount >=0; iCount--)
    {
        m_QueueTile.Add(iCount);
    }

    printf("Orange\t0\n");
    printf("Green\t1\n");
    printf("Purple\t2\n");
    printf("Earth\t3\n");
    printf("Saturn\t4\n");
    printf("Moon\t5\n");
    printf("IO\t6\n");
    printf("Mars\t7\n\n");
    return;
}
cBoard::~cBoard()
{
    return;
}

void cBoard::PrintOut()
{
    INT iCount;
    INT iCountDown;

    for (iCount = 1; iCount <= 16; iCount+=4)
    {
        for (iCountDown = iCount; iCountDown <iCount+4; iCountDown++)
        {
            printf("  %d \t", m_cTileList[m_bStack[iCountDown]].get_Up());
        }
        printf("\n");
        for (iCountDown = iCount; iCountDown < iCount+4; iCountDown++)
        {
            printf("%d %d %d\t", m_cTileList[m_bStack[iCountDown]].Get_Left(),m_bStack[iCountDown],m_cTileList[m_bStack[iCountDown]].Get_Right());
        }
        printf("\n");
        for (iCountDown = iCount; iCountDown < iCount+4; iCountDown++)
        {
            printf("  %d \t", m_cTileList[m_bStack[iCountDown]].Get_Down());
        }
        printf("\n");
    }

    printf("\n\nDone\n");

    printf("Total Number of Rotations = %d\n", m_uiNumOfRotations);
    printf("Total Number of Pushes = %d\n", m_uiNumOfPushes);
    printf("Total Number of Pops = %d\n", m_uiNumOfPops);

    iCountDown = 0;

    for (iCount = 0; iCount < 16; iCount++)
    {
        printf("m_cTileList[%d].m_uiTotalRotations == %d\n", iCount, m_cTileList[iCount].m_uiTotalRotations);
        iCountDown += m_cTileList[iCount].m_uiTotalRotations;
    }
    printf("total Rotations = %d\n", iCountDown);
    printf("Average number of Rotations = %d\n", iCountDown / 16);


}
