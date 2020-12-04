
/*	program ADVENT0.C					*\
\*	execution will read the four adventure text files	*\
\*	files; "advent1.txt", "advent2.txt", "advent3.txt" &	*\
\*	"advent4.txt".  it will create the file "advtext.h"	*\
\*	which is an Index Sequential Access Method (ISAM)	*\
\*	header to be #included into "advent.c" before the	*\
\*	header "advdef.h" is #included.				*/

#define radix 10
#include    <strsafe.h>
#include	<stdio.h>	/* drv = 1.1st file 2.def 3.A	*/
#include	<stdlib.h>
#include	"advent.h"

int Output(FILE * fDest, int iCurrent, FILE * fdCurrent, char * cType);

int main(int argc, char *argv[])
{

    FILE *fdx[5];
    char cFile[20];
    int iCount;

    char *cType[] = {"LOC", "LOC", "OBJ", "MSG"};

    fdx[0] = fopen("advtext.h", "w");       // Open the Destination file
    if (fdx[0] == NULL)
    {
        printf("Sorry, I can't open advtext.h...\n");
        exit(1);
    }


    // Open all of the source txt files
    for (iCount=1; iCount < 5; iCount++)
    {
        StringCbPrintf(cFile,20,"advent%d.txt", iCount);
        printf("Opening %s\n", cFile);
        fdx[iCount] = fopen(cFile, "r");

        if (!fdx[iCount])
        {
            printf("Sorry, I can't open advent%d.txt...\n",iCount);
            exit(1);
        }
    }

    fprintf(fdx[0], "\n/");
    fprintf(fdx[0], "*\theader: ADVTEXT.H\t\t\t\t\t*/\n\n\n");


    for (iCount=1; iCount < 5; iCount++)
    {
        
        printf("\nProcessing advent%d.txt\n", iCount);
        if (!Output(fdx[0],iCount, fdx[iCount], cType[iCount-1] ))
        {
            exit(0);
        }
    }

    printf("\n");

    return 0;
}                       /* main		*/


int Output(FILE * fDest, int iCurrent, FILE * fdCurrent, char * cType)
{
    char  itxt[255], lstr[12];
    int   iCnt =-1;                 // because we delay printing the first item set this counter to -1

    // this is keep the commas right
    lstr[0] = '\0';

    fprintf(fDest, "long\tg_lIdx%d[MAX%s] = {\n\t", iCurrent,cType);
    while (fgets(itxt, 255, fdCurrent))
    {
        printf(".");
        if (itxt[0] == '#')
        {
            // just to keep the commas looking good
            if (lstr[0] != '\0')
            {
                fprintf(fDest, "%s,",lstr);
            }
            // Convert the string's position in the file to an ascii string
            StringCbPrintf(lstr,12,"%d",ftell(fdCurrent));

            // this is just to beautify the output.. putting only 4 items on a line
            if (iCnt++ == 4)
            {
                fprintf(fDest, "\n\t");
                iCnt = 0;
            }           /* if (iCnt)	*/
        }               /* if (itxt[0])	*/
    }                   /* while fgets	*/

    fprintf(fDest, "%s\n\t};\n\n",lstr);

    return 1;

}
