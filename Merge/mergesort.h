#ifndef __MERGESORT_
#define __MERGESORT_

typedef struct _NODE *PNODE;

typedef struct _NODE
{
    INT iNumber;
    PNODE pNext;
} NODE;

PNODE Merge(PNODE pFirst, PNODE pSecond);
VOID Sort(PNODE pList);

VOID DumpList(PNODE pList);

#endif
