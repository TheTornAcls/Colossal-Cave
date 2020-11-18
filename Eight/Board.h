#ifndef __BOARD_H
#define __BOARD_H

class cBoard
{
public:
        cBoard();
        ~cBoard();

        BOOL NextHealthy(int *, int *);
        VOID Heal(int, int);
        INT  ReturnTotalThreat();
        VOID PrintBoard();

        VOID Take(int, int);
        VOID PrintThreat();

private:     
    INT nhX;
    INT nhY;

    INT m_Board[8][8];
    INT m_Queens[8];

    INT m_CurrentQueen;

    void priv_Vertical(int, int, int);
    void priv_Horizontal(int, int, int);
    void priv_NegativeDiagonal(int, int, int);
    void priv_PositiveDiagonal(int ,int, int);

    VOID Poison(int, int);

};

#endif
