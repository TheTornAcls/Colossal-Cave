// mystring.cpp
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 2B

#include <iostream>
using namespace std;

#include <string.h>
#include "MyString.h"


// Constructors
CMyString::CMyString()
  : m_iStringLen(0),
    m_iBufferLen(0),
    m_pszBuffer(NULL)
{
}

CMyString::CMyString(const char* pszIn)
  : m_iStringLen(NULL == pszIn ? 0 : strlen(pszIn)),
    m_iBufferLen(0 == m_iStringLen ? 0 : m_iStringLen + 1),
    m_pszBuffer(0 == m_iBufferLen ? NULL : new char[m_iBufferLen])
{
    if (m_pszBuffer)
        strcpy(m_pszBuffer, pszIn);
}

CMyString::CMyString(const char *pszIn, int iLen)
  : m_iStringLen(NULL == pszIn ? 0 : strlen(pszIn)),
    m_iBufferLen((m_iStringLen + 1 > iLen) ? m_iStringLen + 1 : iLen),
    m_pszBuffer(new char[m_iBufferLen])
{
    if (m_pszBuffer)
        strcpy(m_pszBuffer, pszIn);
}

// Copy Constructor: Do deep copy
CMyString::CMyString(const CMyString& strIn)
  : m_iStringLen(strIn.m_iStringLen),
    m_iBufferLen(0 == m_iStringLen ? 0 : m_iStringLen + 1),
    m_pszBuffer(0 == m_iBufferLen ? NULL : new char[m_iBufferLen])
{
    if (m_pszBuffer)
        strcpy(m_pszBuffer, strIn.m_pszBuffer);
}

// Destructor
CMyString::~CMyString()
{
    delete [] m_pszBuffer;
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

char CMyString::GetChar(int iIndex)
{
    // Returns 0 if user tries to access past the logical end of the string
    if ((iIndex < m_iStringLen) && (iIndex >= 0))
        return m_pszBuffer[iIndex];
    else
        return '\0';
}

void CMyString::Print()
{
    if (m_pszBuffer)
        cout << m_pszBuffer;

    cout << endl;
}

void CMyString::Print(int cChars)
{
    int nPrintChars = (cChars < m_iStringLen) ? cChars : m_iStringLen;

    for (int i = 0; i < nPrintChars ; i++)
        cout << m_pszBuffer[i];

    cout << endl;
}

const char* CMyString::CharPtr()
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

CMyString CMyString::MidStr(int iStart)
{
    // Implement this MidStr by calling the other one
    return MidStr(iStart, m_iStringLen - iStart);
}

CMyString CMyString::MidStr(int iStart, int cChar)
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

int CMyString::Len()
{
    return m_iStringLen;
}

int CMyString::BufferLen()
{
    return m_iBufferLen;
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

bool CMyString::operator!()
{
    return (m_iStringLen == 0) ? false : true;
}

bool CMyString::operator==(const CMyString& strIn)
{
    return (strcmp(m_pszBuffer, strIn.m_pszBuffer) ? false : true);
}

bool CMyString::operator!=(const CMyString& strIn)
{
    return !(*this == strIn);   // !(operator==(strIn));
}

CMyString CMyString::operator()(int iStart)
{
    return MidStr(iStart, m_iStringLen - iStart);
}

CMyString CMyString::operator()(int iStart, int cChar)
{
    return MidStr(iStart, cChar);
}

char  CMyString::operator[](int iIndex)
{
    return GetChar(iIndex);
}


// Non-member Functions
int Compare(CMyString& strFirst, CMyString& strSecond)
{
    return strcmp(strFirst.CharPtr(), strSecond.CharPtr());
}

CMyString Concat(CMyString& strFirst, CMyString& strSecond)
{
    CMyString strTemp(strFirst.CharPtr(), strFirst.Len() + strSecond.Len() + 1);
    strTemp.Append(strSecond);
    return strTemp;
}

// Non-member Binary Operator Functions
CMyString operator+(const char* pszIn, CMyString& strIn)
{
    CMyString strTemp(pszIn);
    strTemp.Append(strIn);
    return strTemp;
}

ostream& operator<<(ostream& out, CMyString& strIn)
{
    if (0 != strIn.Len())
        out << strIn.CharPtr();

    return out;
}