// Test4.cpp - Graphics Classes client file
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 4 Answer


#include <iostream>
using namespace std;

#include "Graphics.h"


const int COUNT_OF_OBJECTS = 5;

void main()
{
    CPoint p(10, 10);                               // point
    CLine l(CPoint(115, 135), CPoint(200, 250));    // start, end points
    CRectangle r(CPoint(60, 60), CPoint(100, 130)); // opposite corners
    CCircle c(CPoint(140, 140), 50);    // center point and radius
    CSquare s(CPoint(105, 105), 80);    // point and length of one side

    // Declare and initialize array of base class pointers
    CObj *CArray[COUNT_OF_OBJECTS];

    CArray[0] = &p;
    CArray[1] = &c;
    CArray[2] = &l;
    CArray[3] = &r;
    CArray[4] = &s;

    // Call virtual Draw function thru CObj pointers
    for (int i = 0; i < COUNT_OF_OBJECTS; i++)
    {
        CArray[i]->Draw();
        cout << endl;
    }

    // Call virtual Move function thru CObj pointers
    for (i = 0; i < COUNT_OF_OBJECTS; i++)
        CArray[i]->Move(20, -20);

    // Call virtual Draw function again thru CObj pointers
    for (i = 0; i < COUNT_OF_OBJECTS; i++)
    {
        CArray[i]->Draw();
        cout << endl;
    }
}