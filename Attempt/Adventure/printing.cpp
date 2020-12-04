#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "printing.h"

cPrinting::cPrinting()          
{
    
    m_hHandle= GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(m_hHandle, &csbiInfo);
    m_wOldColorAttrs = csbiInfo.wAttributes;

    //m_wBackGround = csbiInfo.wAttributes & (BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);

}

cPrinting::~cPrinting()          
{                    
    if (NULL != m_hHandle)
    {
        CloseHandle(m_hHandle);
    }
}

void cPrinting::ConOutputError(LPSTR cString, ...)
{
    va_list args;

    va_start(args, cString);

    SetConsoleTextAttribute(m_hHandle, FOREGROUND_RED | FOREGROUND_INTENSITY );
    vprintf(cString, args);
    SetConsoleTextAttribute(m_hHandle, m_wOldColorAttrs );
    va_end(args);
}

void cPrinting::ConOutput(LPSTR cString, ...)
{
    va_list args;

    va_start(args, cString);

    SetConsoleTextAttribute(m_hHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
    vprintf(cString, args);
    SetConsoleTextAttribute(m_hHandle, m_wOldColorAttrs );
    va_end(args);

}

void cPrinting::ConOutputChar(CHAR cInput)
{

    SetConsoleTextAttribute(m_hHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY );
    //SetConsoleTextAttribute(m_hHandle, (~(m_wBackGround >> 4) & 0xF) | m_wBackGround);
    printf_s("%c", cInput);
    SetConsoleTextAttribute(m_hHandle, m_wOldColorAttrs );
}
