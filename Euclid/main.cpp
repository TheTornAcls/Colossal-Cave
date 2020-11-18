#include <stdio.h>
#include <windows.h>

// An interative version of Euclid's algorithm for finding the Greatest         
// Common Factor
int GCF(int iFirst, int iSecond)
{
    int iDiv;

    do
    {
        iDiv = iFirst % iSecond;
        iFirst = iSecond;
        iSecond = iDiv;

    } while (iSecond);

    return iFirst;
}

// A Recursive version of Euclid's algorithm for finding the Greatest Common 
// Factor
int RGCF(int iFirst, int iSecond)
{
    if (0 == iSecond)
    {
        return iFirst;
    }
    else
    {
        return RGCF(iSecond, iFirst % iSecond);
    }

}

// This is a Brute Force method for finding the Greatest Commond Factor
int BFGCF(int iFirst, int iSecond)
{
    int GCF = 1;
    int iCount = 2;
    int iMax;

    if (iFirst < iSecond)
    {
        iMax = iFirst;
    }
    else
    {
        iMax = iSecond;
    }


    while (iCount <= iMax)
    {
        if ( !(iFirst % iCount) && !(iSecond % iCount))
        {
            iFirst /=iCount;
            iSecond /= iCount;
            GCF *= iCount;
        }
        else
        {
            iCount++;
        }
    }

    return GCF;
}


int main(int argc, char *argv[])
{

    printf("GCF = %d\n", GCF(864, 45000000));
    printf("GCF = %d\n\n", GCF(81,36));
    printf("GCF = %d\n\n", GCF(-81,36));

    printf("RGCF = %d\n", RGCF(864, 45000000));
    printf("RGCF = %d\n\n", RGCF(81,36));

    printf("BFGCF = %d\n", BFGCF(864, 45000000));
    printf("BFGCF = %d\n", BFGCF(81,36));

}

