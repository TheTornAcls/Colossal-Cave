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

        for (iCount = 0; iCount < (argc - 1); iCount++)
        {
            iList[iCount] = atoi(argv[iCount+1]);
        }
    }

    return iList;

}

int ComputeTime(int iAmount, int iValues[])
{
    int iCount = iAmount - 1;
    int iTotalLat = 0;

    while (iCount > 2)
    {
        if ((2*iValues[1]) < (iValues[0] + iValues[iCount - 1]))
        {
            iTotalLat = iTotalLat + iValues[0] + 2*iValues[1] + iValues[iCount];
            printf("1st iTotalLat = %d\n",iTotalLat);
        }
        else
        {
            iTotalLat = iTotalLat + 2*iValues[0] + iValues[iCount - 1] + iValues[iCount];
            printf("2nd iTotalLat = %d\n",iTotalLat);
        }
        iCount-=2;
    }

    //OK if we have 3 items the total Latency is the sum of the 3
    if (iCount == 2)
    {
        iTotalLat = iTotalLat + iValues[0] + iValues[1] + iValues[2];
        printf("3rd iTotalLat = %d\n",iTotalLat);
    }
    else
    {
        // add the last element
        iTotalLat += iValues[iCount];
        printf("4th iTotalLat = %d\n",iTotalLat);
    }

    return iTotalLat;
}

int main(int argc, char *argv[])
{

    int * iList;

    int iTotalLat;

    if (argc > 1)
    {
        iList = Convert(argc,argv);

        if (NULL != iList)
        {

            iTotalLat = ComputeTime(5,iList );

            printf("Total Latency == %d\n",iTotalLat);

            LocalFree(iList);
        }
    }
    
    return 1;
}

