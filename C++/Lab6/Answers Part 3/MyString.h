// MyString.h
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 6


#pragma once

class CMyString
{
// Non-member Functions declared as friends
friend int Compare(const CMyString& strFirst, const CMyString& strSecond);
friend CMyString Concat(const CMyString& strFirst, const CMyString& strSecond);
friend CMyString operator+(const char* pszIn, const CMyString& strIn);
friend ostream& operator<<(ostream& out, const CMyString& strIn);

public:
    // Constructors & Destructor
    CMyString(const char *pszIn = NULL);    // use default argument
    CMyString(const char *pszIn, int iLen);
    CMyString(const CMyString& strIn);

    // Destructor implemented as implicit inline
    ~CMyString()
    {
        delete [] m_pszBuffer;
    }

    // Public Methods
    void SetChar(int iIndex, char ch);
    char GetChar(int iIndex) const;     // const
    void Print() const;                 // const
    void Print(int cChars) const;       // const
    const char* CharPtr() const;        // const

    const CMyString& Append(const CMyString& strIn);
    const CMyString& Append(const char *pszIn);

    CMyString MidStr(int iStart) const; // const
    CMyString MidStr(int iStart, int cChars) const; // const
    inline int Len() const;         // explicit inline and const
    inline int BufferLen() const;   // explicit inline and const

    // Assignment Operators
    const CMyString& operator=(const CMyString& strIn);
    const CMyString& operator=(const char* pszIn);

    // Additional Overloaded Operators
    const CMyString& operator+=(const CMyString& strIn);
    const CMyString& operator+=(const char* pszIn);

    CMyString operator+(const CMyString& strIn) const;  // const
    CMyString operator+(const char* pszIn) const;   // const
    bool operator!() const;                         // const
    bool operator==(const CMyString& strIn) const;  // const
    bool operator!=(const CMyString& strIn) const;  // const
    CMyString operator()(int start) const;          // const
    CMyString operator()(int start, int cChar) const;   // const
    char operator[](int iIndex) const;              // const

    // Static Member functions
    static int GetDefaultBufferLength();
    static void SetDefaultBufferLength(int iLen);

private:
    int m_iStringLen;
    int m_iBufferLen;
    char *m_pszBuffer;
    static int s_iDefaultBufferLength;  // static member data
};


// Implement these "Get" functions as explicit inlines
inline int CMyString::Len() const
{
    return m_iStringLen;
}

inline int CMyString::BufferLen() const
{
    return m_iBufferLen;
}


// Turn all these non-member functions into "friend"
// functions of the CMyString class.  These prototypes
// are unchanged (other than adding "const" to the
// parameters), but their implementations will be able
// to use CMyString private data.

// Non-member Functions
int Compare(const CMyString& strFirst, const CMyString& strSecond);
CMyString Concat(const CMyString& strFirst, const CMyString& strSecond);

// Non-member Binary Operator Functions
CMyString operator+(const char* pszIn, const CMyString& strIn);
ostream& operator<<(ostream& out, const CMyString& strIn);
