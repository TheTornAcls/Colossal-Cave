// MyArray.h: CMyArray template class header
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 6, Part 3

#pragma once

template <typename T> class CMyArray
{
public:
    CMyArray<T>()
      : m_pElements(NULL),
        m_iLength(0)
    { }
        
    CMyArray<T>(int iLen)
      : m_pElements(new T[iLen]),
        m_iLength(iLen)
    { }

    int Length()
    {
        return m_iLength;
    }

    inline void Length(int iLen);
    inline T& operator[](int iIndex);

private:
    T*  m_pElements;  // Pointer to array of elements
    int m_iLength;
};


template <typename T> inline void CMyArray<T>::Length(int iLen)
{
    T *pT = NULL;

    if (0 != iLen)
        pT = new T[iLen];

    if ((NULL != pT) && (NULL != m_pElements))
    {
        int iMin = (iLen < m_iLength) ? iLen : m_iLength;

        // Preserve array elements using assignment operator
        for (int i = 0; i < iMin; i++)
            pT[i] = m_pElements[i];
    }

    delete [] m_pElements;

    m_pElements = pT;
    m_iLength = iLen;
}

template <typename T> inline T& CMyArray<T>::operator[](int iIndex)
{
    return m_pElements[iIndex];
}
