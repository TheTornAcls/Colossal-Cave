// Test1.cpp: Test program for Lab 1
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 1


#include <iostream>
using namespace std;

#include "MyString.h"


void main(void)
{
    CMyString a, b("Hello there!");
    a = "Hiya! ";

    a.Print();
    b.Print();

    CMyString c;
    a.Append(b);
    c = a;
    c.Print();

    if (Compare(a, c))
        cout << "a != b" << endl;
    else
        cout << "a == b" << endl;

    c.MidStr(3, 6).Print();
    CMyString d = c.MidStr(5);

    c.Print(11);
    c.Print(120);

    cout << "Length of c is " << c.Len() << endl;
    cout << "The 2nd character of a is " << a.GetChar(1) << endl;
    a.SetChar(1, 'X');
    cout << "The 2nd character of a is now " << a.GetChar(1) << endl;

    CMyString x("01234567890123456789012345678901234567890123456789"
                "01234567890123456789012345678901234567890123456789");

    a = "01234567890123456789012345678901234567890123456789"
        "01234567890123456789012345678901234567890123456789";

    b.Append("01234567890123456789012345678901234567890123456789"
             "01234567890123456789012345678901234567890123456789");

    c.SetChar(79, '*');
    cout << "c.GetChar(79) is " << c.GetChar(79) << endl;
}
