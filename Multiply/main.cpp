#include <stdio.h>
#include <windows.h>

int Multiply(int iFirst, int iSecond)
{
    int iProduct = 0;
    BOOL bNegative = FALSE;

    if (0 > iFirst)
    {
        bNegative = !bNegative;
        // Make it a positive number
        iFirst = 0 - iFirst;
    }

    if (0 > iSecond)
    {
        bNegative = !bNegative;
        iSecond =0 - iSecond;
    }

    while (0 != iFirst)
    {
        if (iFirst & 0x1)
        {
            iProduct += iSecond;
        }

        iSecond = iSecond << 1;
        iFirst = iFirst >> 1;
        printf("iSecond = %d\n",iSecond);
        printf("iFirst = %d\n",iFirst);

    }

    // is the final number a negative number??
    if (bNegative)
    {
        return 0 - iProduct;
    }
    else
    {
        return iProduct;
    }
}

int BitCount(int iInput)
{
    int iCount = 0;



    while (iInput)
    {
         iInput = iInput&(iInput - 1); 
         iCount++;
        
    }

    return iCount;
}

int main(int argc, char *argv[])
{
    int iFirst;
    int iSecond;
    int iProduct;     


    printf("%c",78);
    printf("%c",111);
    printf("%c",119);
    printf("%c",32);
    printf("%c",72);
    printf("%c",105);
    printf("%c",110);
    printf("%c",103);
    printf("%c\n\n",78);

    if (3 > argc)
    {
        printf("Not enough parameters\n");
    }
    else
    {
        iFirst = atoi(argv[1]);
        iSecond = atoi(argv[2]);

        iProduct = Multiply(iFirst, iSecond);

        printf("%d * %d = %d\n", iFirst, iSecond, iProduct);

    }

    printf("%d has %d\n",iFirst, BitCount(iFirst) );
    printf("%d has %d\n",iSecond, BitCount(iSecond) );
    printf("0x%X has %d\n",iProduct, BitCount(iProduct) );


}

