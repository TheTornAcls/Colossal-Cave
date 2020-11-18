// Graphics.cpp - Graphics Classes implementation file
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 4 Answer


#include <iostream>
using namespace std;

#include "Graphics.h"

void CPoint::Draw()
{
    cout << "CPoint::Draw()" << endl;
    cout << "X = " << m_iX << ", Y = " << m_iY << endl;
}

void CPoint::Move(int xOffset, int yOffset)
{
    m_iX += xOffset;
    m_iY += yOffset;
}

void CLine::Draw()
{
    cout << "CLine::Draw()" << endl;
    cout << "Draw line from: X = " << m_ptStart.GetX()
        <<  ", Y = " << m_ptStart.GetY() << endl;
    cout << "To end point: X = " << m_ptEnd.GetX()
        <<  ", Y = " << m_ptEnd.GetY() << endl;
}

void CLine::Move(int xOffset, int yOffset)
{
    m_ptStart.Move(xOffset, yOffset);
    m_ptEnd.Move(xOffset, yOffset);
}

void CRectangle::Draw()
{
    cout << "CRectangle::Draw()" << endl;
    cout << "Draw lines using: X = " << m_ptFirst.GetX()
        <<  ", Y = " << m_ptFirst.GetY() << endl;
    cout << "And second point: X = " << m_ptSecond.GetX()
        <<  ", Y = " << m_ptSecond.GetY() << endl;
}

void CRectangle::Move(int xOffset, int yOffset)
{
    m_ptFirst.Move(xOffset, yOffset);
    m_ptSecond.Move(xOffset, yOffset);
}

void CCircle::Draw()
{
    cout << "CCircle::Draw()" << endl;
    cout << "Draw circle centered at: X = " << m_ptCenter.GetX()
        <<  ", Y = " << m_ptCenter.GetY() << endl;
    cout << "Using radius = " << m_iRadius << endl;
}

void CCircle::Move(int xOffset, int yOffset)
{
    m_ptCenter.Move(xOffset, yOffset);
}

