// CMyString.cpp
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 1

#include <iostream>
using namespace std;

#include <string.h>
#include "MyString.h"


CMyString::CMyString()
  : m_iLen(0)
{
    m_szBuffer[0] = '\0';
}

CMyString::CMyString(char *pszIn)
  : m_iLen(strlen(pszIn) < (MAX_LEN - 1) ? strlen(pszIn) : (MAX_LEN - 1))
{
    strncpy(m_szBuffer, pszIn, m_iLen);
    m_szBuffer[m_iLen] = '\0';      // explicitly add null terminator
}

void CMyString::SetChar(int iIndex, char ch)
{
    // Does not allow access past logical end of the string.  If index is
    // greater than the length of the string then no setting occurs.
    if ((iIndex < m_iLen) && (iIndex >= 0))
    {
        m_szBuffer[iIndex] = ch;

        // If user sets a null terminating character then m_iLen must be reset
        if ('\0' == ch)
            m_iLen = iIndex;
    }
}

char CMyString::GetChar(int iIndex)
{
    // Returns 0 if user tries to access past the logical end of the string
    if ((iIndex < m_iLen) && (iIndex >= 0))
        return m_szBuffer[iIndex];
    else
        return '\0';
}

void CMyString::Print()
{
    cout << m_szBuffer << endl;
}

void CMyString::Print(int cChars)
{
    int nPrintChars = (cChars < m_iLen) ? cChars : m_iLen;

    for (int i = 0; i < nPrintChars ; i++)
        cout << m_szBuffer[i];

    cout << endl;
}

const char* CMyString::CharPtr()
{
    return m_szBuffer;
}

void CMyString::Append(CMyString strIn)
{
    // Implement this Append by calling the (const char *) version
    Append(strIn.m_szBuffer);
}

void CMyString::Append(const char *pszIn)
{
    strncat(m_szBuffer, pszIn, MAX_LEN - m_iLen - 1);
    m_iLen = strlen(m_szBuffer);
}

CMyString CMyString::MidStr(int iStart)
{
    // Implement this MidStr by calling the other one
    return MidStr(iStart, m_iLen - iStart);
}

CMyString CMyString::MidStr(int iStart, int cChar)
{
    CMyString strTemp;
    int iMaxChar = 0;

    // Returns the remainder of the string from the start position if user
    // tries to access past the logical end of the original string
    if ((iStart >= 0) && (cChar >= 0) && (iStart < m_iLen))
    {
        iMaxChar = (cChar + iStart > m_iLen) ? m_iLen - iStart : cChar;
        strncpy(strTemp.m_szBuffer, m_szBuffer + iStart, iMaxChar);
        strTemp.m_szBuffer[iMaxChar] = '\0';
        strTemp.m_iLen = iMaxChar;
    }

    return strTemp;
}

int CMyString::Len()
{
    return m_iLen;
}



// Non-member Compare function
int Compare(CMyString strFirst, CMyString strSecond)
{
    return strcmp(strFirst.CharPtr(), strSecond.CharPtr());
}
