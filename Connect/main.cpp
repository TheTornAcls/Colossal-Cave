#include <stdio.h>
#include <windows.h>

#include "pop.h"
#include "binary.h"
#include "connect.h"

int main(int argc, char *argv[])
{
    cBinTree cTest;
    cBinary cBase;
    int iCount;

	int num1 = 5;
	int num2 = num1++;

	string 

    for (iCount=1; iCount < argc; iCount++)
    {
        cTest.AddItem(argv[iCount][0]);
        cBase.AddItem(argv[iCount][0]);
    }

    printf("Testing Level Printing for Test\n");
    cTest.PrintLevel();
    printf("Testing Level Printing for Test\n");
    cBase.PrintLevel();

    cTest.PrintChildren();
    cBase.PrintChildren();




    return 1;
}

