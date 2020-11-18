// Test6C.cpp: Test program for Lab 6, Part 3
//
// Test of Template class CMyArray
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 6, Part 3


#include <iostream>
using namespace std;

#include "mystring.h"
#include "SAExcept.h"
#include "MyArray.h"


void main(void)
{
    CMyArray<int> iList(20);
    CMyArray<CMyString> strArray(10);

    iList[3] = 42;
    cout << "iList[3] = " << iList[3] << endl;

    strArray[8] = CMyString("Hi there!");
    cout << "strArray[8] = " << strArray[8] << endl;

    cout << "strArray.Length = " << strArray.Length() << endl;

    strArray.Length(15);
    cout << "strArray.Length = " << strArray.Length() << endl;
    cout << "strArray[8] = " << strArray[8] << endl;

    strArray[8].Append(" and Hello to you.");
    cout << "strArray[8] = ";
    strArray[8].Print();
}