#include <stdio.h>
#include <windows.h>

int Count(int iInput)
{
    int iCount =0;
    while (iInput)
    {
        iInput= iInput & (iInput-1);
        iCount++;
    }
    return iCount;
}


int main(int argc, char *argv[])
{
    int iNumber;
    if (argc <2)
    {
        printf("Please give me a number\n");
    }
    else
    {
        INT iCount = 1;
        while (iCount < argc)
        {
            iNumber = atoi(argv[iCount]);
            printf("%x \n",iNumber);
            printf("Value Returned = %d\n\n", Count(iNumber));
            iCount++;
        }
    }
}

