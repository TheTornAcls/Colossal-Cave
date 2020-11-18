#include "stdafx.h"

cThreadPrint::cThreadPrint()
{
    // create my events
    m_hBufferFull = CreateEvent(NULL, TRUE, TRUE,"BUFFERFULL");
    m_hBufferEmpty = CreateEvent(NULL, TRUE, FALSE,"BUFFEREMPTY");

    // Initialize my indexes into the buffer
    m_iRead = 0;
    m_iWrite = 0;

    // setup my critical section
    InitializeCriticalSection(&csBlock);

}

cThreadPrint::~cThreadPrint()
{
    // Close both events
    CloseHandle(m_hBufferFull);
    CloseHandle(m_hBufferEmpty);

    // And close the Critical Section
    DeleteCriticalSection(&csBlock);
}

DWORD WINAPI cThreadPrint::Read(LPVOID lpParam)
{
    int iCount;
    int iNumber;
    if (NULL != lpParam)
    {
        // pull off the incoming parameter and make it an int
        iCount = *(int *)lpParam;

        for (iNumber = 0; iNumber < iCount; iNumber++)
        {
            // wait if the buffer is empty
            WaitForSingleObject(m_hBufferEmpty, INFINITE);

            // enter in critical section protecting the READ and WRITE indexes
            EnterCriticalSection(&csBlock);
            printf("%d ", m_iBuffer[m_iRead]);

            // we removed an item so go ahead and set the event
            SetEvent(m_hBufferFull);

            // Update the Read Index
            m_iRead = (m_iRead + 1) % BUFFSIZE;
            // make the print out look all pretty
            if (0 == m_iRead)
            {
                printf("\n");
            }
            // check to see if our buffer is Empty
            if (m_iRead == m_iWrite)
            {
                ResetEvent(m_hBufferEmpty);
            }

            // release the critical section
            LeaveCriticalSection(&csBlock);

            // have some fun.  with this in place we should see write finish 10 items before 
            // the read thread
            Sleep(1000);
        }
    }
    printf("Exit Read\n");
    return 0;
}

DWORD WINAPI cThreadPrint::Write(LPVOID lpParam)
{
    int iCount;
    int iNumber;
    if (NULL != lpParam)
    {
        // pull off the incoming parameter and make it an int
        iCount = *(INT *)lpParam;
        for (iNumber = iCount; iNumber > 0; iNumber--)
        {
            // wait if the buffer is full
            WaitForSingleObject(m_hBufferFull, INFINITE);

            // enter in critical section protecting the READ and WRITE indexes
            EnterCriticalSection(&csBlock);

            m_iBuffer[m_iWrite] = iNumber;
            // We added an item so go ahead and set the event
            SetEvent(m_hBufferEmpty);

            // update the Write Index
            m_iWrite = (m_iWrite+1) % BUFFSIZE;

            // Check to see if our buffer is FULL
            if (m_iWrite == m_iRead)
            {
                ResetEvent(m_hBufferFull);
            }

            // Release the critical Section
            LeaveCriticalSection(&csBlock);


        }
    }
    printf("Exit Write\n");
    return 0;
}
