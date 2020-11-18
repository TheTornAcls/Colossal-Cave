// Test3B.cpp: Test program for Lab 3, Part 2
//
// Test of the Employee and Manager Classes
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 3, Part 2


#include <iostream>
using namespace std;

#include "Manager.h"


void main()
{
    CEmployee empNewHire;
    empNewHire.SetName("Fred Farkle");
    empNewHire.SetHireDate("05/31/94");
    empNewHire.SetEndDate("(active)");
    empNewHire.SetJobTitle("Accountant");
    empNewHire.SetSalary(45000);
    empNewHire.Print();
    cout << endl;

    CEmployee empAnalyst("Billie Sue Sunshine",
                         "11/11/99",
                         "(active)",
                         "Analyst",
                         48500.0);
    empAnalyst.Print();
    cout << endl;

    CManager mgrTyrant("Cornelius Grumpp",
                       "11/11/99",
                       "(active)",
                       "Manager",
                       "Accounting & Finance",
                       53600.0);
    mgrTyrant.Print();
    cout << endl;

    mgrTyrant.FireEmployee(empNewHire, "12/25/00");
    empNewHire.Print();
    cout << endl;

    CEmployee empBee;
    empBee = mgrTyrant.HireEmployee("Beulah Busybee",
                                    "01/01/01",
                                    "Auditor",
                                    39000);
    empBee.Print();
    cout << endl;

    mgrTyrant.SetJobTitle("Grand Exalted Overlord");
    mgrTyrant.SetSalary(88000.0);
    mgrTyrant.Print();
}