#include <iostream>
#include "exception.h"

const char* IllegalOperation::what()
{
    return "Illegal Operation";
}

const char* OutOfMemory::what()
{
    return "Out of Memory";
}
