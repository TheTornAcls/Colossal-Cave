// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "heap.hxx"

cHeap::cHeap()
{
    iEnd = 0;
}

cHeap::~cHeap()
{
}

void cHeap::Add(int iElement)
{
    if (iEnd < (HEAPSIZE))
    {
        AddNotFull(iElement);
    }
    else 
    {
        AddFull(iElement);
    }

    return;
}

void cHeap::DumpHeap()
{
    int iCount;
    int iDiv = 1;

    for (iCount = 0; iCount < iEnd; iCount++)
    {
        printf("%i ", iHeap[iCount]);
        if (0 == ((iCount +2 ) & (iCount +1)))
        {
            printf("\n");
            iDiv *= 2;
        }
    }
    if ((iEnd + 1 ) & (iEnd ))
    {
        printf("\n");
    }
    printf("iEnd  = %i\n",iEnd);
    printf("\n");
}

void cHeap::AddFull(int iElement)
{
    int iCurrentPos= 0;
    bool bDone = false;

    if (iElement > iHeap[iCurrentPos])
    {

        while (!bDone && (iCurrentPos < HEAPSIZE))
        {
            int iLeft = (iCurrentPos * 2) +1;
            int iRight = (iCurrentPos *2) + 2;

            if (iLeft > HEAPSIZE )
            {
                bDone = true;
            }
            else if ((iHeap[iLeft] < iElement) && (iHeap[iLeft] <= iHeap[iRight]))
            {
                // the item sinks down the left hand side of the tree
                iHeap[iCurrentPos] = iHeap[iLeft];
                iCurrentPos = iLeft;
            }

            else if ((iHeap[iRight] < iElement) && (iHeap[iRight] < iHeap[iLeft]))
            {
                // The item sinks down the right hand side of the tree.
                iHeap[iCurrentPos] = iHeap[iRight];
                iCurrentPos = iRight;
            }
            else
            {
                // the item is floating
                bDone = true;
            }

        }

        // insert the element
        iHeap[iCurrentPos] = iElement;
    }
}

void cHeap::AddNotFull(int iElement)
{
    int iParent;
    int iCurrent = iEnd;

    bool bDone = false;

    iEnd++;

    while ( !bDone && (iCurrent != 0) )
    {
        iParent = (iCurrent -1 ) / 2;

        if (iHeap[iParent] >  iElement)
        {
            // the item floats to the parent
            // and the parent sinks to the child leaf
            iHeap[iCurrent] = iHeap[iParent];

            iCurrent = iParent;

        }
        else
        {
            bDone = true;
        }
    }

    iHeap[iCurrent] = iElement;

}

int main()

{
    cHeap cHeapTest;
    int iCount;

    for (iCount = 0; iCount < 50; iCount++)
    {
        //printf("Adding %i\n", 50 - iCount);
        //cHeapTest.Add(50 - iCount);
        cHeapTest.DumpHeap();
        cHeapTest.Add(iCount*3);
        cHeapTest.Add(149 - iCount*2);

    }

    cHeapTest.DumpHeap();

	cHeap anotherTest;
	anotherTest.Add(50);
	anotherTest.Add(100);
	anotherTest.Add(90);
	anotherTest.Add(25);
	anotherTest.DumpHeap();
    
        return(0);
}
