#include <iostream>
using namespace std;

#include "mystring.h"

void TestOperators();

void main(void)
{
    CMyString a, b("Hello there!");

    // New stuff to check static members
    int iDef = CMyString::GetDefaultBufferLength();
    cout << "Default Length: " << iDef << endl;
    cout << "Buffer length of a: " << a.BufferLen() << endl;

    // Set default buffer length to large number
    CMyString::SetDefaultBufferLength(0);
    iDef = CMyString::GetDefaultBufferLength();
    cout << "New Default Length: " << iDef << endl;
    CMyString e;
    cout << "Buffer length of e: " << e.BufferLen() << endl;

    // Can call static function using an existing object
    e.SetDefaultBufferLength(-20);
    iDef = CMyString::GetDefaultBufferLength();
    cout << "New New Default Length: " << iDef << endl;

    a = "Hiya! ";

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

    // Create a constant object
    const CMyString cstrConstant("This string cannot be changed");
    cout << cstrConstant << endl;
    // cstrConstant = c;
    // cstrConstant.Append(" New stuff");
    // cstrConstant.SetChar(3, 'Z');

    TestOperators();
}

void TestOperators()
{
    CMyString a("Hello "), b("Help "), c, d, e("Non null object");

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
