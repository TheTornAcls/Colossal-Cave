// employee.h
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 3, Part 2


#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

#include <iostream>
using namespace std;

#include "mystring.h"


class CEmployee
{
public:
    // Constructors
    CEmployee();
    CEmployee(const CMyString& strName,
              const CMyString& strHireDate,
              const CMyString& strEndDate,
              const CMyString& strJobTitle,
              float fSalary);

    // Copy Constructor
    CEmployee(const CEmployee& empIn);

    // Assignment Operator
    const CEmployee& operator=(const CEmployee& empIn);

    // Public Member Function
    void Print();

    // Public Member Functions for Get/Set data (implemented inline)
    void SetName(const CMyString& strName)
    {
        m_strName = strName;
    }

    void SetHireDate(const CMyString& strHireDate)
    {
        m_strHireDate = strHireDate;
    }

    void SetEndDate(const CMyString& strEndDate)
    {
        m_strEndDate = strEndDate;
    }

    void SetJobTitle(const CMyString& strJobTitle)
    {
        m_strJobTitle = strJobTitle;
    }

    void SetSalary(float fSalaryIn)
    {
        if (fSalaryIn > 0)
            m_fSalary = fSalaryIn;
    }

    CMyString GetName()
    {
        return m_strName;
    }

    CMyString GetHireDate()
    {
        return m_strHireDate;
    }

    CMyString GetEndDate()
    {
        return m_strEndDate;
    }

    CMyString GetJobTitle()
    {
        return m_strJobTitle;
    }

    float GetSalary()
    {
        return m_fSalary;
    }

protected:
    CMyString m_strName;
    CMyString m_strHireDate;
    CMyString m_strEndDate;
    CMyString m_strJobTitle;
    float     m_fSalary;
};

#endif  // #define EMPLOYEE_H_