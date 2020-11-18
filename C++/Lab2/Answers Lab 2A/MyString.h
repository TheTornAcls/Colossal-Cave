// mystring.h
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 2A

#ifndef _MYSTRING_H_
#define _MYSTRING_H_

class CMyString
{
public:
    // Constructors & Destructor
    CMyString();
    CMyString(const char *pszIn);
    CMyString(const char *pszIn, int iLen);
    CMyString(const CMyString& strIn);
    ~CMyString();

    // Public Methods
    void SetChar(int iIndex, char ch);
    char GetChar(int iIndex);
    void Print();
    void Print(int cChars);
    const char* CharPtr();
    void Append(const CMyString& str);
    void Append(const char *pszIn);
    CMyString MidStr(int iStart);
    CMyString MidStr(int iStart, int cChars);
    int Len();
    int BufferLen();

    // Assignment Operators
    const CMyString& operator=(const CMyString& strIn);
    const CMyString& operator=(const char* pszIn);

private:
    int m_iStringLen;
    int m_iBufferLen;
    char *m_pszBuffer;
};

// Non-member Functions
int Compare(CMyString& strFirst, CMyString& strSecond);
CMyString Concat(CMyString& strFirst, CMyString& strSecond);

#endif  // #ifdef _MYSTRING_H_
