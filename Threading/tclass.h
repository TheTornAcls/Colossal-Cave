#ifndef _TCLASS_H
#define _TCLASS_H

#define BUFFSIZE 10

class cThreadPrint
{
public:
    cThreadPrint();
    ~cThreadPrint();

    DWORD WINAPI Read(LPVOID);
    DWORD WINAPI Write(LPVOID);

private:
    HANDLE m_hBufferFull;
    HANDLE m_hBufferEmpty;

    INT m_iBuffer[BUFFSIZE];

    int m_iWrite;
    int m_iRead;

    CRITICAL_SECTION csBlock;
};

#endif
