#include <stdio.h>
#include <windows.h>

// we read in the list from the command line, allocate enough space and convert the characters to numbers            
int Convert(int argc, char *argv[])
{
    int iNickels = 0;
    int iDimes = 0;
    int iQuarters = 0;
    int iTotal = 0;
    int iTestVal =0;

        int iCount;
        int iNewNum;

        for (iCount = 1; iCount < argc ; iCount++)
        {

            iNewNum = atoi(argv[iCount]);
            switch (iNewNum)
            {
            case 5:
                iNickels++;
                iTotal+=5;
                break;
            case 10:
                iDimes++;
                iTotal+=10;
                break;
            case 25:
                iQuarters++;
                iTotal+=25;
                break;
            default:
                printf("Ignoring %s\n",argv[iCount]);
                break;
                    
            } // end of switch

        } // end of for


    if (0 != (iTotal % 30))
    {
        printf("Error: sum not multiple of 30\n");
        return 1;
    }

    for (iCount = 0; iCount < iQuarters; iCount++ )
    {
        printf(" 5\t25\n");
        iTestVal+=30;
    }
    iNickels -=iQuarters;

    printf("\n");
    for (iCount = 0; iCount < (iDimes / 3); iCount++)
    {
        printf(" 10\t10\t10\n");
        iTestVal+=30;
    }

    iDimes = (iDimes % 3);

    for (iCount = 0; iCount < (iNickels / 6); iCount++)
    {
        printf(" 5\t5\t5\t5\t5\t5\n");
        iTestVal+=30;
    }

    iNickels = (iNickels % 6);

    if (iNickels == 4)
    {
        printf(" 5\t5\t5\t5\t10\n");
        iTestVal+=30;
    }

    if (iNickels == 2)
    {
        printf(" 5\t5\t10\t10\n");
        iTestVal+=30;
    }

    //printf("iTestVal = %d\n",iTestVal);
    //printf("iTotal = %d\n",iTotal);

    //printf("Total Number %d\n",iList[0]);
    return 1;


}


int main(int argc, char *argv[])
{

    int iRet;
    if (argc > 1)
    {
        // convert the list of command line arguments to an integer array
        iRet = Convert(argc,argv);

    }
    
    return 1;
}

