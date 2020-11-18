// Employee.cpp
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 3, Part 2

#include <iostream>
using namespace std;

#include "Employee.h"


// Constructors
CEmployee::CEmployee()
  : m_strName(),
    m_strHireDate(),
    m_strEndDate(),
    m_strJobTitle(),
    m_fSalary(0.0f)
{
}

CEmployee::CEmployee(const CMyString& strName,
                     const CMyString& strHireDate,
                     const CMyString& strEndDate,
                     const CMyString& strJobTitle,
                     float fSalary)
  : m_strName(strName),
    m_strHireDate(strHireDate),
    m_strEndDate(strEndDate),
    m_strJobTitle(strJobTitle),
    m_fSalary(fSalary)
{
}

// Copy Constructor
CEmployee::CEmployee(const CEmployee& empIn)
  : m_strName(empIn.m_strName),
    m_strHireDate(empIn.m_strHireDate),
    m_strEndDate(empIn.m_strEndDate),
    m_strJobTitle(empIn.m_strJobTitle),
    m_fSalary(empIn.m_fSalary)
{
}

// Assignment Operator
const CEmployee& CEmployee::operator=(const CEmployee& empIn)
{
    m_strName     = empIn.m_strName;
    m_strHireDate = empIn.m_strHireDate;
    m_strEndDate  = empIn.m_strEndDate;
    m_strJobTitle = empIn.m_strJobTitle;
    m_fSalary     = empIn.m_fSalary;

    return *this;
}

// Public Print Method
void CEmployee::Print()
{
    cout << "Name:\t\t" << m_strName << endl;
    cout << "Hire Date:\t" << m_strHireDate << endl;
    cout << "End Date:\t" << m_strEndDate << endl;
    cout << "Job Title:\t" << m_strJobTitle << endl;
    cout << "Salary:\t\t$" << m_fSalary << endl;
}
