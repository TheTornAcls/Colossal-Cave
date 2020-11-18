// Graphics.h - Graphics Classes header file
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 4


// TODO: Reword this explanatory text!

// In a real class you would have multiple constructors to deal with each 
// possible way the user would try to create objects.  Due to convenience issues
// this example assumes that: x, y coordinates are passed for a point, a centerpoint 
// and radius are passed for a circle, two points passed for a rectangle, etc.


#pragma once    // prevent multiple inclusion


class CObj
{
public:
    virtual void Draw() = 0;
    virtual void Move(int xOffset, int yOffset) = 0;
};

class CPoint : public CObj
{
public:
    // Inline Constructor
    CPoint(int x, int y)
      : m_iX(x), m_iY(y)
    { }

    // Virtual functions' implementation
    virtual void Draw();
    virtual void Move(int xOffset, int yOffset);

    // Public Members
    int GetX()
    {
        return m_iX;
    }

    int GetY()
    {
        return m_iY;
    }

private:
    int m_iX;
    int m_iY;
};

class CLine : public CObj
{
public:
    // Inline Constructor
    CLine(CPoint ptStart, CPoint ptEnd)
      : m_ptStart(ptStart), m_ptEnd(ptEnd)
    { }

    virtual void Draw();
    virtual void Move(int xOffset, int yOffset);

private:
    CPoint m_ptStart;
    CPoint m_ptEnd;
};


class CRectangle : public CObj
{
public:
    // Inline Constructor
    CRectangle(CPoint ptFirst, CPoint ptSecond)
      : m_ptFirst(ptFirst), m_ptSecond(ptSecond)
    { }

    virtual void Draw();
    virtual void Move(int xOffset, int yOffset);

private:
    CPoint m_ptFirst;
    CPoint m_ptSecond;
};

class CSquare : public CRectangle
{
public:
    // Inline Constructor
    CSquare(CPoint ptUpperLeft, int iSize)
      : CRectangle(ptUpperLeft,
                   CPoint(ptUpperLeft.GetX() + iSize, ptUpperLeft.GetY() + iSize))
    { }
};

class CCircle : public CObj
{
public:
    // Inline Constructor
    CCircle(CPoint ptCenter, int iRadius)
      : m_ptCenter(ptCenter),
        m_iRadius(iRadius)
    { }

    virtual void Draw();
    virtual void Move(int xOffset, int yOffset);

private:
    CPoint  m_ptCenter;
    int     m_iRadius;
};


