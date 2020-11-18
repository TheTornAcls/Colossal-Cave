// MyString.h
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 1


#ifndef _MYSTRING_H_
#define _MYSTRING_H_

const int MAX_LEN = 80;

class CMyString
{
public:
    // Contructors
    CMyString();
    CMyString(char *pszIn);

    // Public Methods
    void SetChar(int iIndex, char ch);
    char GetChar(int iIndex);
    void Print();
    void Print(int cChars);
    const char* CharPtr();
    void Append(CMyString str);
    void Append(const char *pszIn);
    CMyString MidStr(int iStart);
    CMyString MidStr(int iStart, int cChars);
    int Len();

private:
    int  m_iLen;
    char m_szBuffer[MAX_LEN];
};

// Non-member Compare function
int Compare(CMyString strFirst, CMyString strSecond);


#endif
