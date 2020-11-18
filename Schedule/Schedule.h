#ifndef __SCHEDULE_H
#define SCHEDULE_H

#include <stdio.h>
#include <windows.h>

#define BUSY 0
#define AWAY 1
#define FREE 2

typedef struct _TNODE *PTNODE;

typedef struct _TNODE
{
    INT iStart;
    INT iEnd;
    INT iStatus;

} TNODE;


typedef struct _NODE *PNODE;

typedef struct _NODE
{
    INT iStart;
    INT iEnd;
    INT iStatus;

    PNODE pNext;
} NODE;


class cSchedule
{
public:
    cSchedule();
    ~cSchedule();

    HRESULT Input(PTNODE ptList, INT iNumber);
    void Print();
    HRESULT ReturnSummary(PTNODE pTNODE, PINT piSize);

private:
    NODE m_Head;
    INT  iNumElements;

    PNODE CreateNode(INT iStart, INT iEnd, INT iStatus);
    HRESULT Add(INT iStart, INT iEnd, INT iStatus);
    HRESULT Add2(INT iStart, INT iEnd, INT iStatus);
    void DumpNode(PNODE pInput);


};

#endif  SCHEDULE_H
