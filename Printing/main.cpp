#include <stdio.h>
#include <windows.h>

#include "printc.h"

int main()
{

    printc("testing\n");
    printc("list %c the\n", 'v');

    printc("hex %x hex\n", 16);
    printc("hex %x hex\n", -15);
    printc("hex %x hex\n", 15);
    printc("hex %x hex\n", 0);

    printc("oct %o oct\n", 64);
    printc("oct %o oct\n", -64);
    printc("oct %o oct\n", 72);
    printc("oct %o oct\n", 63);
    printc("oct %o oct\n", 0);

    printc("dec %d dec\n",0);
    printc("dec %d dec\n",10);
    printc("dec %d dec\n",-10);

    printc("uns %u uns\n",0);
    printc("uns %u uns\n",10);
    printc("uns %u uns\n",-10);

    printc("bin %b bin\n",0);
    printc("bin %b bin\n",10);
    printc("bin %b bin\n",-10);
    printc("bin %b bin\n",7);
    printc("bin %b bin\n",8);
    printc("bin %b bin\n",1024);

    printc("dec %% dec %w\n",10, 20);
    printc("dec %% dec %w\n");

    printc("dec %d oct %o hex %x char %c string %s end\n", 15,15,15,'f',"the");

    printc("string %s a string\n","within");
    printc(NULL);
    printc("string %s a string\n",NULL);

    printc("bin %b bin\ndec %d dec\t",0xFFFFFFFF,0xFFFFFFFF);
    printc("uns %u uns\noct %o oct\t",0xFFFFFFFF,0xFFFFFFFF);
    printc("hex %x hex\n",0xFFFFFFFF,0xFFFFFFFF);

    printc("a\nb\nc\n");

    printc("the %d rain %x in %c Spain %o falls %u mainly %b on \n");
   
    return 1;


}
