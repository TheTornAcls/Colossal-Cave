#include <stdio.h>
#include <windows.h>
#include "Screen.h"


int main(int argc, char *argv[])
{
    cRectangle crTest;
    HRESULT hr;
    RECTANGLE rRect;
    
    hr = crTest.FindRect(0,0,NULL, 0);
    
    if (FAILED(hr))
    {
        printf("Call Failed as we expected\n");
    }
    hr = crTest.FindRect(120, 120, &rRect, sizeof(RECTANGLE));
    if (SUCCEEDED(hr))
    {
        printf("Start (%d, %d)\nEnd(%d, %d)\n", rRect.xStart, rRect.yStart, rRect.xEnd, rRect.yEnd);
    }
    else
    {
        printf("Error Wrong Values\n");
    }
    
    hr = crTest.FindRect(20, 20, &rRect, sizeof(RECTANGLE));
    if (SUCCEEDED(hr))
    {
        printf("Start (%d, %d)\nEnd(%d, %d)\n", rRect.xStart, rRect.yStart, rRect.xEnd, rRect.yEnd);
    }
    
}


