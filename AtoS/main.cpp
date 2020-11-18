#include "stdafx.h"

short CountBits()
{
    short sTemp = -1;
    int iCount = 0;

    while (sTemp)
    {
        sTemp = sTemp & (sTemp -1);
        iCount++;
    }
    
    sTemp = 0;
    iCount--;

    while (iCount > 0)
    {
        sTemp = sTemp << 1;
        sTemp++;
        iCount--;
    }

    return  sTemp;
}

short ConvertString(LPSTR lpString)
{
    short sNumber=0;
    short sCurrent;

    // determine the largest non-negative number
    short sMax = CountBits();
    short sDifference;

    while ((*lpString >= '0' ) && (*lpString <= '9'))
    {
        sCurrent = *lpString - '0';

        sDifference = sMax - (sNumber * 10);

        //if ((sDifference < sCurrent) || (sDifference > 9))
        if (sDifference >= sCurrent)
        {
             sNumber = (sNumber * 10) + sCurrent;
             lpString++;
        }
        else
        {
            // We have overflow
            return sNumber;
        }

    }

    return sNumber;
}

int main()
{
    printf("Number is %d\n", ConvertString("32767"));
    printf("Number is %d\n", ConvertString("32768"));
    return 1;
}

