#ifndef _MYSTRING
#define _MYSTRING

#include <windows.h>

const int MAX_LEN = 80;

class CMyString
{
public:
    CMyString();                    //
    CMyString(LPSTR lpstrIn);       //
    ~CMyString();                   //

    void Print();                   //
    void Print(INT iStart);         //

    void Append(LPSTR lpstrAdd);
    void Append(CMyString cAppend);
    INT Len();
                                    //
    CHAR GetChar(INT iAddress);
    void SetChar(INT iAddress, CHAR newChar);    //

    CMyString MidStr(INT iStart);
    CMyString MidStr(INT iStart, INT iEnd);

private:
    CHAR m_String[MAX_LEN];
    INT m_iLength;


};

// Non-member Compare function
int Compare(CMyString strFirst, CMyString strSecond);

#endif
