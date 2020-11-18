#include <stdio.h>
#include <windows.h>
#include "Remove.h"

// The small size of the hash table limits the type of characters you can input.
// You can only use the letters a-z (no spaces, capitals, or numbers.)

// Make the hash table bigger and capable of handling collisions.


// My Hash Table
BOOL bHash[26];

VOID AddToHash(CHAR cAdd)
{
    // OK through some error checking into the mix
    if ( (cAdd >= 'a') && (cAdd <= 'z'))
    {
        bHash[cAdd - 'a'] = TRUE;
    }
    return;
}

// search the Hash table for that character value.
// return true if found
BOOL SearchHash(CHAR cSearch)
{
    // Does the character land in our HASH Table
    if ( (cSearch >= 'a') && (cSearch <= 'z'))
    {
        return bHash[cSearch - 'a'];
    }

    return FALSE;
}

// RemoveChar takes lpSource and removes the list of characters specified by lpstrList
int RemoveChar(LPSTR lpSource, LPSTR lpRemoveList)
{      
    // Peform removal in place

    INT i;
    int iCount = 0;
    LPSTR lpRead;   // used to keep track of the current character
    LPSTR lpWrite;  // used to keep track of the current location to write


    // Using a HASH table to keep track of characters to remove
    for (i= 0; i < 26; i++)
    {
        bHash[i] = FALSE;
    }

    //Walk through the remove list adding each character to the HASH table.
    while ('\0' != *lpRemoveList)
    {
        AddToHash(*lpRemoveList);
        lpRemoveList++;
    }
    // Do removal "in Place"
    
    // Count the number of characters removed
    lpRead = lpSource;
    lpWrite = lpSource;

    while ('\0' != *lpRead)
    {
        //if the character is NOT in the Hash table then keep it
        if (!SearchHash(*lpRead))
        {
            *lpWrite = *lpRead;
            lpWrite++;
        }
        else
        {
            iCount++;
        }

        lpRead++;
    }

    *lpWrite = '\0';

    // How many characters did we remove
    return iCount;
}


int main(int argc, char *argv[])
{
    INT iRet;

    // did they provide us a Source String
    if (argc < 2)
    {
        printf("You need to provide a Source string and Removal List\n");
        return 0;
    }
    //Did they provide us with a Remove List
    if (argc < 3)
    {
        printf("You need to provide a Removal List with your Source Strings\n");
        return 0;
    }

    printf("Source = %s\n", argv[1]);
    printf("Remove = %s\n", argv[2]);

    iRet = RemoveChar(argv[1], argv[2]);

    printf("%d characters were removed from the string.\n", iRet);

    if ('\0' == *argv[1])
    {
        printf("\nThe New String is:\n<empty>\n");
    }
    else
    {
        printf("\nThe New String is:\n%s\n",argv[1]);
    }

    return 1;
}

