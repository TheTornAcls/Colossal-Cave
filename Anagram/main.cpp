#include <stdio.h>
#include <windows.h>

#include "hash.h"

int main(int argc, char *argv[])
{
    cHash myCHash;
    INT iCount;

    printf("Anagram String Finder Version 1.0\n\n");

    for (iCount = 1; iCount < argc; iCount++)
    {
        myCHash.Add(argv[iCount]);
    }

    myCHash.DumpHash();

    return 1;

}

