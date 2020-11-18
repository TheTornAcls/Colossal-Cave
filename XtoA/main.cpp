#include <stdio.h>
#include <windows.h>

// 0 1 2 3 4 5 6 7 8 9 0 1
// A B C D E F G H I J A B
//                     A

bool MyItoA(int iMyInt, LPSTR lpDest)
{
    LPSTR lpTemp;

    if (0> iMyInt)
    {
        *lpDest = '-';
        iMyInt *=-1;
        lpDest++;
    }

    lpTemp = lpDest;

    do
    {
        
            *lpTemp = 'A' + (iMyInt % 5);
            
            iMyInt = (iMyInt) / 5;
            
            iMyInt--;
            

        lpTemp++;
    
    } while (0 <= iMyInt);
    

    *lpTemp = '\0';
    lpTemp--;

    // OK Now we have to swap

    char cTemp;
    while (lpTemp > lpDest)
    {
        cTemp = *lpTemp;
        *lpTemp = *lpDest;
        *lpDest = cTemp;

        // move the head and tail pointers
        lpTemp--;
        lpDest++;
    }


    return true;
}

int main()
{

    CHAR cOutput[20];
    
    int iCount;

    MyItoA(0, cOutput);
    printf("0 %s\n\n", cOutput);
    MyItoA(26, cOutput);
    printf("26 %s\n\n", cOutput);
    MyItoA(25, cOutput);
    printf("25 %s\n\n", cOutput);
    MyItoA(27, cOutput);
    printf("27 %s\n\n", cOutput);
    MyItoA(52, cOutput);
    printf("52 %s\n\n", cOutput);
    MyItoA(53, cOutput);
    printf("53 %s\n\n", cOutput);
    MyItoA(-10, cOutput);
    printf("-10 %s\n\n", cOutput);
    MyItoA(9930, cOutput);
    printf("9930 %s\n\n", cOutput);
    MyItoA(12340, cOutput);
    printf("12340 %s\n\n", cOutput);
    
    MyItoA(626, cOutput);
    printf("626 %s\n\n", cOutput);
    MyItoA(627, cOutput);
    printf("627 %s\n\n", cOutput);
    
    //for (iCount =0; iCount < 703; iCount++)
    for (iCount =0; iCount < 800; iCount++)
    {
        MyItoA(iCount, cOutput);
        printf("%d\t%s\n",iCount, cOutput);
        
    }
    
    return 1;
}

