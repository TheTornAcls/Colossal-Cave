#include <stdio.h>
#include <windows.h>
#include "FindReplace.h"

// this routine makes multiple passes through the string
// 1st pass: count the number of times lpFind occurs in lpSource
// 2nd pass: find the length of the replacement string - this will be used to allocate memory +1 for the NULL
// 3rd pass: find and replace lpFind with lpReplace


int ReplaceString(LPSTR lpSource, LPSTR lpFind, LPSTR lpReplace)
{
    INT iCount = 0;

    LPSTR lpTemp;
    LPSTR lpSrcTemp;

    LPSTR lpFindTemp;

    INT iLengthFind = 0;

   

    if ((NULL == lpSource) || (NULL == lpFind) || (NULL == lpReplace))
    {
        return 0;
    }

    // count the number of instances
    lpSrcTemp = lpSource;

    // 1st pass through the string

    while ('\0' != *lpSrcTemp)
    {
        lpTemp = lpSrcTemp;
        lpFindTemp = lpFind;

        while (('\0' != *lpTemp) && ('\0' != *lpFindTemp) && !(*lpTemp - *lpFindTemp))
        {
            lpTemp++;
            lpFindTemp++;
        }

        if (('\0'== *lpFindTemp) && (lpFindTemp != lpFind))
        {
            iCount++;
            iLengthFind = lpFindTemp - lpFind;
            lpSrcTemp = lpTemp;
        }
        else
        {
            lpSrcTemp++;
        }
    }

    // OK if iLengthFind equals 0 then lpFind was not in the Search string

    INT iLengthSrc = lpSrcTemp - lpSource;

    // OK we are cheating here by using strlen, but we need to know the length of the replace string.
    INT iLengthReplace = strlen(lpReplace);

    // Length of new string is the length of old string + 
    // (the difference between the lengths of old and new strings) times the number of occurances lpFind is in lpSource
    // plus one character for the null termination
    INT iSize = iLengthSrc + iCount*(iLengthReplace - iLengthFind) + 1;

    LPSTR lpNewString = (LPSTR)LocalAlloc(LPTR, sizeof(CHAR)*iSize); 

    if (NULL == lpNewString)
    {
        return 0;
    }

    LPSTR lpNewTemp= lpNewString;

    // 2nd pass through the string!!

    // Go back through the string to find and replace lpFind with lpReplace
    lpSrcTemp = lpSource;

    while ('\0' != *lpSrcTemp)
    {
        lpTemp = lpSrcTemp;
        lpFindTemp = lpFind;

        while (('\0' != *lpTemp) && ('\0' != *lpFindTemp) && !(*lpTemp - *lpFindTemp))
        {
            lpTemp++;
            lpFindTemp++;
        }

        // What if the Src string only contains a '\0'??
        if (('\0'== *lpFindTemp) && (lpFindTemp != lpFind))
        {
            // OK we found the string
            lpFindTemp = lpReplace;

            // copy the new string
            while ('\0' != *lpFindTemp)
            {
                *lpNewTemp = *lpFindTemp;
                lpNewTemp++;
                lpFindTemp++;
            }

            // adjust the pointers to keep searching for the next occurance
            lpSrcTemp = lpTemp;
        }
        else
        {
            // not found copy the current character over to the new string and increment both string pointers
            *lpNewTemp = *lpSrcTemp;
            lpSrcTemp++;
            lpNewTemp++;
        }
    }
    // Add the Null termination of the string
    *lpNewTemp = *lpSrcTemp;

     printf("New String = %s\n",lpNewString);

     LocalFree(lpNewString);


    return iCount;
}


int main(int argc, char *argv[])
{
    INT iRet;

    // did they provide us a string
    if (argc < 4)
    {
        printf("You need to provide a string\n");
        return 0;
    }

    printf("String 1= %s\n", argv[1]);
    printf("String 2 =%s\n", argv[2]);
    printf("String 3 =%s\n\n", argv[3]);

    iRet = ReplaceString(argv[1], argv[2], argv[3]);

    printf("Count = %d\n", iRet);

    return 1;
}

