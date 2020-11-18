#include <windows.h>
#include <stdio.h>

#include "detect.h"


int main (int argc, char * argv[])
{

        CDetection cDetect;

        cDetect.Initialize(argc, argv);

        cDetect.ModifyInfo();

        cDetect.WriteBatchFile();

    return 1;
}
