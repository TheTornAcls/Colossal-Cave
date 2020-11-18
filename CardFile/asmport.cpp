#include "cardfile.h"

/************************************************************************/
/*                                                                      */
/*  Windows Cardfile                                                    */
/*  (c) Copyright Microsoft Corp. 1985, 2004 - All Rights Reserved      */
/*                                                                      */
/************************************************************************/
int Frename(char *src, char *dst)
{
    return MoveFile(src,dst);
}

BOOL Fdelete(char *src)
{
    return DeleteFile(src);
}

int mylread(HANDLE fh, void *buf,int n)
{
    DWORD dwRead;
    BOOL bRet;
   

    bRet = ReadFile(fh, buf, n, &dwRead, NULL);

    return dwRead;
}

int myread(HANDLE fh, void *buf, int n)
{
    return mylread(fh, buf, n);
}

int mylwrite(HANDLE fh, void *buf, int n)
{
    DWORD dwWritten;
    return WriteFile(fh,buf,n, &dwWritten, NULL);

}

int mywrite(HANDLE fh, void *buf, int n)
{
    return mylwrite(fh, buf, n);
}   

