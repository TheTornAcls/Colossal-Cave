#include <stdio.h>
#include <windows.h>

//****************************************************
//
// Convert the incoming integer to the LCD
// since we don't have an LCD digit display we will
// convert the number to a character
//
//      6
//     5 4 
//      3 
//     2 1
//      0
//****************************************************
char Mapping(int iInput)
{
    char iRet;

      switch (iInput)
      {
      case 0:
          iRet = '0';
          //iRet = 0x77
          break;
      case 1:
      case -1:
          iRet = '1';
          //iRet = 0x12
          break;
      case 2:
      case -2:
          iRet = '2';
          //iRet = 0x93
          break;
      case 3:
      case -3:
          iRet = '3';
          //iRet = 0x5B
          break;
      case 4:
      case -4:
          iRet = '4';
          //iRet = 0x3A
          break;
      case 5:
      case -5:
          iRet = '5';
          //iRet = 0x6B
          break;
      case 6:
      case -6:
          iRet = '6';
          //iRet = 0x6F
          break;
      case 7:
      case -7:
          iRet = '7';
          //iRet = 0x52
          break;
      case 8:
      case -8:
          iRet = '8';
          //iRet = 0x7F
          break;
      case 9:
      case -9:
          iRet = '9';
          //iRet = 0x7B
          break;
      default:
          printf("Default %d\n",iInput);
          iRet = '0';
          //iRet = 0x77
      }

      return iRet;

}

void LCD(int iInput, char ** ppOutput)
{
    BOOL bNegative = FALSE;
    INT  iChars = 1;
    INT  iTemp = iInput;
    PCHAR pString = NULL;
    HANDLE handleHeap;

    handleHeap = GetProcessHeap();
    if (NULL != handleHeap)
    {
        // Is the number negative
        if (iInput < 0)
        {
            bNegative = TRUE;
            iChars++;
        }

        // Count the number of digits
        do
        {
           iChars++;
           iTemp = iTemp / 10;
        } while ( 0 != iTemp );

        // Allocate the needed buffer
        pString = (PCHAR) HeapAlloc(handleHeap, HEAP_ZERO_MEMORY, sizeof(CHAR)*iChars);

        // If the Alloc succeeded continue on
        if (NULL != pString)
        {
            iChars--;
            pString[iChars] = '\0'; 

            do
            {
                iChars --;
                pString[iChars] = Mapping(iInput % 10);
                iInput /= 10;
            } while ( 0 != iInput );

            if (bNegative)
            {
                iChars--;
                pString[iChars] = '-';
                //pString[iChars] = 0x8
            }

        }                                 
    }

    *ppOutput = pString;
    return;
}

int main()
{
    PCHAR pString = NULL;
    HANDLE handleHeap = GetProcessHeap();

    LCD(9345, &pString);

    if (NULL != pString)
    {
        printf("The new string is %s\n", pString);
        //LocalFree(pString);
        HeapFree(handleHeap, 0, pString);
    }
    LCD(-459, &pString);

    if (NULL != pString)
    {
        printf("The new string is %s\n", pString);
        HeapFree(handleHeap, 0, pString);
    }

}
