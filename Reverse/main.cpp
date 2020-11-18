#include <stdio.h>
#include <windows.h>
#include "reverse.h"


int main(int argc, char *argv[])
{
    cReverse cTest;
    char String[] = "The Rain In Spain";
    char *pCount;

    pCount = String;

    printf("%s\n",String);

    while ('\0' != *pCount)
    {
        cTest.AddChar(*pCount);
        pCount++;
    }

    cTest.DumpString();
    
    return 1;
}

