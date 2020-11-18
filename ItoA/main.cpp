#include <stdio.h>
#include <windows.h>

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

    
    // go through and pull the least significant digit off the integer
    do
    {
        *lpTemp = '0' + (iMyInt % 10);

        iMyInt = iMyInt / 10;
        lpTemp++;

    } while (0 != iMyInt  );

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

    MyItoA(0, cOutput);
    printf("1st %s\n\n", cOutput);
    MyItoA(-10, cOutput);
    printf("2nd %s\n\n", cOutput);
    MyItoA(9930, cOutput);
    printf("3rd %s\n\n", cOutput);
    MyItoA(12340, cOutput);
    printf("4th %s\n\n", cOutput);
    
    return 1;
}

