/*      program ADVENT0.C                                       *\
\*      execution will read the four adventure text files       *\
\*      files; "advent1.txx", "advent2.txx", "advent3.txx" &    *\
\*      "advent4.txx". it will create the file "advtext.h"      *\
\*      which is an Index Sequential Access Method (ISAM)       *\
\*      header to be #included into "advent.c" before the       *\
\*      header "advdec.h" is #included.                         */


#include        <stdlib.h>
#include        <stdio.h>
#include        <stdarg.h>
#include        <string.h>
#include        "..\Adventure\ADVENT.H"

static int      fError                  = 0;
static FILE     *filIndex, *filTxt1, *filTxt2, *filTxt3, *filTxt4;


void FileError (const char *pszMessage, const char *pszFileName) 
{
        char    szError [FILENAME_MAX + 100];

        strcpy_s (szError, FILENAME_MAX + 100, pszMessage);
        strcat_s(szError, FILENAME_MAX + 100, " ");
        strcat_s(szError, FILENAME_MAX + 100, pszFileName);

        perror (szError);
}


FILE *OpenFile (const char *pszName, const char *pszMode) {

        FILE    *pfil    = fopen (pszName, pszMode);

        if (!pfil) 
		{
                FileError ("Error opening", pszName);
                fError = 1;
        }

        return pfil;

}


void OutputS (const char *psz) {

        if (fputs (psz, filIndex) < 0) {
                FileError ("Error in output file", ISAM);
                exit (EXIT_FAILURE);
        }

}



void OutputF (const char *pszFormat, ...) {

        va_list         va;

        va_start (va, pszFormat);
        if (vfprintf (filIndex, pszFormat, va) < 0) {
                FileError ("Error in output file", ISAM);
                exit (EXIT_FAILURE);
        }
        va_end (va);

}


void DoFile (FILE *pfil, const char *pszIDX, const char *pszMAX, const char *pszFileName) {

        char            szInput[255];
        char            szOutput[12]    = "";
        register int    cNumbers        = -1;

        OutputF ("long    idx%s[MAX%s] = {\n\t", pszIDX, pszMAX);
        while (fgets (szInput, sizeof szInput, pfil)) 
		{
                if (szInput[0] == '#') 
				{
					if (szOutput[0])
					{
						OutputF("%s,", szOutput);
					}
                        sprintf(szOutput, "%ld", ftell(pfil));
                        if (++cNumbers == 5) 
						{
                                OutputS ("\n\t");
                                cNumbers = 0;
                        }
                }
        }
        if (ferror (pfil)) {
                FileError ("Error in input file", pszFileName);
                exit (EXIT_FAILURE);
        }
        OutputF ("%s\n\t};\n\n", szOutput);

}


void CloseFile (FILE *pfil, const char *pszFileName) {

        if (fclose (pfil)) {
                FileError ("Error closing", pszFileName);
                fError = 1;
        }

}


int main (void) {

        filIndex = OpenFile(ISAM, ISAMMODE);
        filTxt1  = OpenFile(FD1, "r");
        filTxt2  = OpenFile(FD2, "r");
        filTxt3  = OpenFile(FD3, "r");
        filTxt4  = OpenFile(FD4, "r");

		if (fError)
		{
			exit(EXIT_FAILURE);
		}

        OutputS ("/*      header: ADVTEXT.H                                      */\n\n");
        OutputS ("#define TEXTDEF\n\n");

        DoFile (filTxt1, "1", "LOC", FD1);
        DoFile (filTxt2, "2", "LOC", FD2);
        DoFile (filTxt3, "3", "OBJ", FD3);
        DoFile (filTxt4, "4", "MSG", FD4);

        CloseFile (filIndex, ISAM);
        CloseFile (filTxt1, FD1);
        CloseFile (filTxt2, FD2);
        CloseFile (filTxt3, FD3);
        CloseFile (filTxt4, FD4);

        if (fError)
                exit (EXIT_FAILURE);

        exit (EXIT_SUCCESS);

}
