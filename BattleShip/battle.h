#ifndef __BATTLE_H
#define __BATTLE_H

#define BOARDSIZE 16
#define HIT 1
#define MISS 0
#define SUNK 2
#define INVALID 3

typedef struct _NODE *PNODE;

typedef struct _NODE
{
    INT iHits;
} NODE, *PNODE;

class cBoard
{
public:
    cBoard();
    ~cBoard();
    BOOL Add(INT iShip, INT iX[], INT iY[]);
    BYTE Fire(INT iX, INT iY);


private:
    PNODE Board[BOARDSIZE][BOARDSIZE];

};

#endif
