#include "stdafx.h"

int main(int argc, char *argv[])
{

    cBoard MyBoard;
    int x, y;
    BOOL bFound;

    int iCount = GetTickCount();

    bFound = MyBoard.NextHealthy(&x, &y);

    if (bFound)
    {
        printf("x = %d\ny = %d\n", x, y);
        MyBoard.Take(x,y);
        //MyBoard.PrintThreat();
        bFound = MyBoard.NextHealthy(&x, &y);
        if (bFound)
        {
            printf("x = %d\ny = %d\n", x, y);
            MyBoard.Take(x, y);
            MyBoard.PrintThreat();
        }
        else
        {
            printf("Second not found\n");
        }
        MyBoard.Heal(0,0);

    }
    else
    {
        printf("Not Found\n");
    }

    //for (xCount = 0; xCount < 8; xCount++)
    //{
    //
    //}

    MyBoard.PrintThreat();


    printf("Total Time = %i\n", GetTickCount() - iCount);
    return 1;
}
