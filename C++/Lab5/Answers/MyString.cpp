// mystring.cpp
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 5


#include <iostream>
using namespace std;

#include <string.h>
#include "MyString.h"

// Static class data
int CMyString::s_iDefaultBufferLength = 16;


// Constructors  
CMyString::CMyString(const char* pszIn)
  : m_iStringLen(NULL == pszIn ? 0 : strlen(pszIn)),
    m_iBufferLen(0 == m_iStringLen ? s_iDefaultBufferLength : m_iStringLen + 1),
    m_pszBuffer(0 == m_iBufferLen ? NULL : new char[m_iBufferLen])
{
    if ((NULL != m_pszBuffer) && (NULL != pszIn))
        strcpy(m_pszBuffer, pszIn);
}

CMyString::CMyString(const char *pszIn, int iLen)
  : m_iStringLen(NULL == pszIn ? 0 : strlen(pszIn)),
    m_iBufferLen((m_iStringLen + 1 > iLen) ? m_iStringLen + 1 : iLen),
    m_pszBuffer(new char[m_iBufferLen])
{
    if ((NULL != m_pszBuffer) && (NULL != pszIn))
        strcpy(m_pszBuffer, pszIn);
}

// Copy Constructor: Do deep copy
CMyString::CMyString(const CMyString& strIn)
  : m_iStringLen(strIn.m_iStringLen),
    m_iBufferLen(0 == m_iStringLen ? s_iDefaultBufferLength : m_iStringLen + 1),
    m_pszBuffer(0 == m_iBufferLen ? NULL : new char[m_iBufferLen])
{
    if ((NULL != m_pszBuffer) && (NULL != strIn.m_pszBuffer))
        strcpy(m_pszBuffer, strIn.m_pszBuffer);
}

void CMyString::SetChar(int iIndex, char ch)
{
    // Does not allow access past logical end of the string.  If index is
    // greater than the length of the string then no setting occurs.
    if ((iIndex < m_iStringLen) && (iIndex >= 0))
    {
        m_pszBuffer[iIndex] = ch;

        // If user sets a null terminating character then m_iStringLen
        // must be reset
        if ('\0' == ch)
            m_iStringLen = iIndex;
    }
}

char CMyString::GetChar(int iIndex) const
{
    // Returns 0 if user tries to access past the logical end of the string
    if ((iIndex < m_iStringLen) && (iIndex >= 0))
        return m_pszBuffer[iIndex];
    else
        return '\0';
}

void CMyString::Print() const
{
    if (m_pszBuffer && (0 != m_iStringLen))
        cout << m_pszBuffer;

    cout << endl;
}

void CMyString::Print(int cChars) const
{
    int nPrintChars = (cChars < m_iStringLen) ? cChars : m_iStringLen;

    for (int i = 0; i < nPrintChars ; i++)
        cout << m_pszBuffer[i];

    cout << endl;
}

const char* CMyString::CharPtr() const
{
    return m_pszBuffer;
}

const CMyString& CMyString::Append(const CMyString& strIn)
{
    // Implement this Append by calling the (const char *) version
    return Append(strIn.m_pszBuffer);
}

const CMyString& CMyString::Append(const char *pszIn)
{
    if (NULL != pszIn)
    {
        int iNewStringLen = m_iStringLen + strlen(pszIn);

        // Grow buffer if needed
        if (iNewStringLen + 1 > m_iBufferLen)
        {
            m_iBufferLen = iNewStringLen + 1;
            char *p = new char[m_iBufferLen];

            if (m_pszBuffer)
                strcpy(p, m_pszBuffer);

            delete [] m_pszBuffer;
            m_pszBuffer = p;
        }

        // Append data from strIn
        strcpy(m_pszBuffer + m_iStringLen, pszIn);
        m_iStringLen = iNewStringLen;
    }

    return *this;
}

CMyString CMyString::MidStr(int iStart) const
{
    // Implement this MidStr by calling the other one
    return MidStr(iStart, m_iStringLen - iStart);
}

CMyString CMyString::MidStr(int iStart, int cChar) const
{
    CMyString strTemp;

    // Returns the remainder of the string from the start position if user
    // tries to access past the logical end of the original string
    if ((iStart >= 0) && (cChar >= 0) && (iStart < m_iStringLen))
    {
        int iNewStringLen = (cChar + iStart > m_iStringLen) ? m_iStringLen - iStart : cChar;

        strTemp.m_iStringLen = iNewStringLen;
        strTemp.m_iBufferLen = iNewStringLen + 1;
        strTemp.m_pszBuffer = new char[strTemp.m_iBufferLen];

        strncpy(strTemp.m_pszBuffer, m_pszBuffer + iStart, iNewStringLen);
        strTemp.m_pszBuffer[iNewStringLen] = '\0';
    }

    return strTemp;
}

// Overloaded Assignment Operators
const CMyString& CMyString::operator=(const CMyString& strIn)
{
    // Check to see that we're assigning a different object
    if (this != &strIn)
    {
        // Call the (const char*) version
        operator=(strIn.m_pszBuffer);
    }

    return *this;
}

const CMyString& CMyString::operator=(const char* pszIn)
{
    if (NULL != pszIn)
    {
        m_iStringLen = strlen(pszIn);

        // Grow buffer in current object if necessary
        if (m_iStringLen + 1 > m_iBufferLen)
        {
            delete [] m_pszBuffer;
            m_iBufferLen = m_iStringLen + 1;
            m_pszBuffer = new char[m_iBufferLen];
        }

        strcpy(m_pszBuffer, pszIn);
    }
    else
    {
        m_iStringLen = 0;

        if (NULL != m_pszBuffer)
            *m_pszBuffer = '\0';
    }

    return *this;
}


// Additional Overloaded Operators
const CMyString& CMyString::operator+=(const CMyString& strIn)
{
    return Append(strIn.m_pszBuffer);
}

const CMyString& CMyString::operator+=(const char* pszIn)
{
    return Append(pszIn);
}

CMyString CMyString::operator+(const CMyString& strIn) const
{
    // Implement this operator+ by calling the other version
    return operator+(strIn.m_pszBuffer);
}

CMyString CMyString::operator+(const char* pszIn) const
{
    // Construct output CMyString using current CMyString
    // plus the correct (larger) buffer size
    CMyString strTemp(m_pszBuffer, m_iStringLen + strlen(pszIn) + 1);
    strTemp.Append(pszIn);
    return strTemp;
}

bool CMyString::operator!() const
{
    return (m_iStringLen == 0) ? false : true;
}

bool CMyString::operator==(const CMyString& strIn) const
{
    return (strcmp(m_pszBuffer, strIn.m_pszBuffer) ? false : true);
}

bool CMyString::operator!=(const CMyString& strIn) const
{
    return !(*this == strIn);   // !(operator==(strIn));
}

CMyString CMyString::operator()(int iStart) const
{
    return MidStr(iStart, m_iStringLen - iStart);
}

CMyString CMyString::operator()(int iStart, int cChar) const
{
    return MidStr(iStart, cChar);
}

char CMyString::operator[](int iIndex) const
{
    return GetChar(iIndex);
}

// Static Member functions
int CMyString::GetDefaultBufferLength()
{
    return s_iDefaultBufferLength;
}

void CMyString::SetDefaultBufferLength(int iLen)
{
    if (iLen >= 0)
        s_iDefaultBufferLength = iLen;
}

// Non-member Functions declared as friends
int Compare(const CMyString& strFirst, const CMyString& strSecond)
{
    // Friend function can access private data
    return strcmp(strFirst.m_pszBuffer, strSecond.m_pszBuffer);
}

CMyString Concat(const CMyString& strFirst, const CMyString& strSecond)
{
    // Friend function can access private data
    CMyString strTemp(strFirst.m_pszBuffer, strFirst.m_iStringLen + strSecond.m_iStringLen + 1);
    strTemp.Append(strSecond);
    return strTemp;
}

// Non-member Binary Operator Functions declared as friends
CMyString operator+(const char* pszIn, const CMyString& strIn)
{
    // Friend function can access private data
    CMyString strTemp(pszIn);
    strTemp.Append(strIn.m_pszBuffer);
    return strTemp;
}

ostream& operator<<(ostream& out, const CMyString& strIn)
{
    // Friend function can access private data
    if (0 != strIn.m_iStringLen)
        out << strIn.m_pszBuffer;

    return out;
}
