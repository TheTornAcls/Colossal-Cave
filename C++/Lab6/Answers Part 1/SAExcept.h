// SAExcept.h: CMyStringAccessException class header file
//
// Copyright (c) Microsoft Corporation, 1997-2000
//
// Microsoft Technical Education
// C++ Programming
// Lab 6

#pragma once

#include <exception>
using namespace std;


class CMyStringAccessException : public exception
{
public:
    // Constructor
    CMyStringAccessException(const char* pszIn = "String access exception")
      : exception(pszIn)
    { }
};