// Test2B.cpp: Test program for Lab 2B
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 2B


#include <iostream>
using namespace std;

#include "mystring.h"

void TestOperators();

void main(void)
{
    CMyString a, b("Hello there!");
    a = "Hiya! ";

    CMyString aCopy(a);

    a.Print();
    b.Print();

    CMyString c;
    a.Append(b);
    c = a;
    c.Print();

    if (Compare(a, c))
        cout << "a != c" << endl;
    else
        cout << "a == c" << endl;

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

    b.SetChar(30, '\0');
    c.Print();
    cout << "String Length of b is now: " << b.Len() << endl;
    cout << "Buffer Length of b is now: " << b.BufferLen() << endl;

    TestOperators();
}

void TestOperators()
{
    CMyString a("Hello "), b("Help "), c, d, e("Non null object");
    CMyString g(NULL, 0);

    e = d;
    c = d;
    c = a + b;
    c.Print();

    c = b + "Me";
    c.Print();

    c = "Hello, " + a;
    c.Print();

    c += b;
    c.Print();

    c += "";
    c += "A new string";
    c = b;

    CMyString f;
    f += "New string";

    cout << c << endl;

    if ((c == b) && (c != a))
        c.Print();

    cout << c(3, 4) << endl;

    cout << c(5) << endl;

    c = "";

    if (!c)
        cout << "Not Empty" << endl;
    else
        cout << "Empty" << endl;

    char ch = 0;

    ch = b[3];
    cout << "ch = '" << ch << "'" << endl;
}
