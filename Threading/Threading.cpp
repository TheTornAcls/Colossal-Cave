// Threading.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// Had problems calling CreateThread with Class Functions                        
// so forced the issue by creating 2 functions, each calls
// the global ctTest directly (either Write or Read
cThreadPrint ctTest;

// the function for reading
DWORD WINAPI ReadProc(LPVOID lpParam)
{
    return ctTest.Read(lpParam);
}

// the function for writing
DWORD WINAPI WriteProc(LPVOID lpParam)
{
    return ctTest.Write(lpParam);
}

int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hTotal[2];
    INT iTotal = 100;


    hTotal[1] = CreateThread(NULL, 0,ReadProc,(LPVOID)&iTotal,0,NULL);
    hTotal[0] = CreateThread(NULL, 0,WriteProc,(LPVOID)&iTotal,0,NULL);

    printf("Waiting for Threads to complete\n");
    WaitForMultipleObjects(2,hTotal,TRUE, INFINITE);
	return 0;
}


