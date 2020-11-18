// manager.h
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 3, Part 2


#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <iostream>
using namespace std;

#include "employee.h"


class CManager : public CEmployee
{
public:
    // Constructors
    CManager();
    CManager(const CMyString& strName,
             const CMyString& strHireDate,
             const CMyString& strEndDate,
             const CMyString& strJobTitle,
             const CMyString& strDeptName,
             float fSalary);

    // Public Methods
    CEmployee HireEmployee(const CMyString& strName,
                           const CMyString& strHireDate,
                           const CMyString& strJobTitle,
                           float fSalaryIn);
    void FireEmployee(CEmployee& emp, const CMyString& strEndDate);

    // Public Methods implemented inline
    void SetDeptName(const CMyString& strDeptName)
    {
        m_strDeptName = strDeptName;
    }

    CMyString GetDeptName()
    {
        return m_strDeptName;
    }

    void Print()
    {
        CEmployee::Print();
        cout << "Department:\t" << m_strDeptName << endl;
    }

protected:
    CMyString m_strDeptName;
};


#endif  // #ifdef _MANAGER_H_