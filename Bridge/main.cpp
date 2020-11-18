#include <stdio.h>
#include <windows.h>

// we read in the list from the command line, allocate enough space and convert the characters to numbers            
int * Convert(int argc, char *argv[])
{
    INT * iList;

    iList = (int *)LocalAlloc(LPTR, sizeof(int)*argc);

    if (NULL != iList)
    {
        int iCount;
        int iNewList = 0;
        int iNewNum;
        int iTemp;

        iList[0] = 0;

        for (iCount = 1; iCount < argc ; iCount++)
        {

            iNewNum = atoi(argv[iCount]);
            if (0 != iNewNum)
            {
                iTemp = iNewList + 1;

                // Just a simple bubble sort. I know, I know it isn't very efficient but it should only
                // be a handful of numbers
                while ((iTemp > 1) && (iNewNum < iList[iTemp - 1]))
                {
                    iList[iTemp] = iList[iTemp - 1];
                    iTemp--;

                }

                // save the number
                iList[iTemp] = iNewNum;

                //printf("Adding %d\n", iNewNum);

                // increment the "pointer" to the end of the list and the total number of elements.
                iNewList++;
                iList[0]++;
            }
        }
    }

    //printf("Total Number %d\n",iList[0]);

    return iList;

}

// the 1st element in the list is number of elements in the list
int ComputeTime( int iValues[])
{
    int iCount = iValues[0];
    int iTotalLat = 0;

    while (iCount > 3)
    {
        if ((2*iValues[2]) < (iValues[1] + iValues[iCount - 1]))
        {
            iTotalLat = iTotalLat + iValues[1] + 2*iValues[2] + iValues[iCount];

            printf("\n%d and %d go over and %d comes back\n", iValues[1], iValues[2],iValues[1]);
            printf("%d and %d go over and %d comes back\n", iValues[iCount -1],iValues[iCount], iValues[2]);

            printf("\nCurrent Latency = %d\n",iTotalLat);
        }
        else
        {
            printf("\n%d and %d go over and %d comes back\n", iValues[1], iValues[iCount],iValues[1]);
            printf("%d and %d go over and %d comes back\n", iValues[iCount -1],iValues[1], iValues[1]);

            iTotalLat = iTotalLat + 2*iValues[1] + iValues[iCount - 1] + iValues[iCount];
            printf("\nCurrent Latency = %d\n",iTotalLat);
        }
        iCount-=2;
    }

    //OK if we have 3 items the total Latency is the sum of the 3
    if (iCount == 3)
    {
        printf("\n%d and %d go over and %d comes back\n", iValues[1], iValues[3],iValues[1]);
        printf("%d and %d go over \n", iValues[1],iValues[2]);

        iTotalLat = iTotalLat + iValues[1] + iValues[2] + iValues[3];
        //printf("3rd iTotalLat = %d\n",iTotalLat);
    }
    else
    {
        if (iCount == 1)
        {
            // in the chance that only 1 value is in the list
            printf("\n%d goes over\n", iValues[1]);
        }              
        else
        {
            printf("\n%d and %d go over\n", iValues[1], iValues[2]);
        }
        // add the last element
        iTotalLat += iValues[iCount];
        //printf("4th iTotalLat = %d\n",iTotalLat);
    }

    return iTotalLat;
}

int main(int argc, char *argv[])
{

    int * iList;

    int iTotalLat;

    if (argc > 1)
    {
        // convert the list of command line arguments to an integer array
        iList = Convert(argc,argv);

        if (NULL != iList)
        {
            // if there are elements in the list then compute the latency                  
            if (0 != iList[0])
            {
                iTotalLat = ComputeTime(iList );

                printf("Total Latency == %d\n",iTotalLat);
            }

            LocalFree(iList);
        }
    }
    
    return 1;
}

