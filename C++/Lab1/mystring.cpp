#include <iostream>
using namespace std;

#include "MyString.h"

CMyString::CMyString()
    :m_iLength(0)
{
    m_String[0] = '\0'; //Null terminate the string
}

CMyString::CMyString(LPSTR lpstrIn)
    :m_iLength( strlen(lpstrIn) < MAX_LEN ? strlen(lpstrIn) : (MAX_LEN -1) )
{
    lstrcpyn(m_String, lpstrIn, m_iLength);
    m_String[MAX_LEN - 1] = '\0';
}

void CMyString::Print()
{
    cout << m_String << endl;
}

void CMyString::Print(INT iStart)
{
  INT iCurrent;
  INT iPrintTo = (iStart > m_iLength ? m_iLength : iStart);

  for (iCurrent = 0; iCurrent < iPrintTo; iCurrent++)
  {
      cout << m_String[iCurrent];
  }

  cout << endl;
}

void CMyString::SetChar(INT iAddress, CHAR newChar)
{
    if ( (iAddress < m_iLength) && (iAddress >=0) )
    {
        m_String[iAddress] = newChar;

        // OK did they send in a NULL character
        if ('\0' == newChar)
        {
            // reset the length
            m_iLength = iAddress;
        }
    }
}

CHAR CMyString::GetChar(INT iAddress)
{
    if ((iAddress < m_iLength) && (iAddress >= 0) )
    {
        return m_String[iAddress];
    }
    else
        return '\0';
}


