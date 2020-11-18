#include <stdio.h>
#include <windows.h>

#include "battle.h"

int main(int argc, char *argv[])
{
    printf("Entering BattleShip\n");
    BYTE bRet;
    cBoard Battle;


    bRet = Battle.Fire(1,1);

    bRet = Battle.Fire(0,0);
    return 1;

}

