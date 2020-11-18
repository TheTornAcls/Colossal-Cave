// manager.cpp
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 3, Part 2


#include <iostream>
using namespace std;

#include "manager.h"

CManager::CManager()
  : CEmployee(),
    m_strDeptName()
{
}


CManager::CManager(const CMyString& strName,
                   const CMyString& strHireDate,
                   const CMyString& strEndDate,
                   const CMyString& strJobTitle,
                   const CMyString& strDeptName,
                   float fSalary)
  : CEmployee(strName, strHireDate, strEndDate, strJobTitle, fSalary),
    m_strDeptName(strDeptName)
{
}

CEmployee CManager::HireEmployee(const CMyString& strName,
                                 const CMyString& strHireDate,
                                 const CMyString& strJobTitle,
                                 float fSalaryIn)
{
    CEmployee empNew(strName,
                     strHireDate,
                     "(active)",
                     strJobTitle,
                     fSalaryIn);
    return empNew;
}

void CManager::FireEmployee(CEmployee& emp, const CMyString& strEndDate)
{
    emp.SetEndDate(strEndDate);
}