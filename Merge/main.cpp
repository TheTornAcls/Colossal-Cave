#include <stdio.h>
#include <windows.h>
#include "mergesort.h"

int main()
{
    PNODE pSearch;
    NODE NodeOne[5];

    NODE NodeTwo[6];

    NodeOne[0].iNumber =6;
    NodeOne[0].pNext = &NodeOne[1];
    NodeOne[1].iNumber =3;
    NodeOne[1].pNext = &NodeOne[2];
    NodeOne[2].iNumber =8;
    NodeOne[2].pNext = &NodeOne[3];
    NodeOne[3].iNumber =2;
    NodeOne[3].pNext = &NodeOne[4];
    NodeOne[4].iNumber =1;
    NodeOne[4].pNext = NULL;

    NodeTwo[0].iNumber =11;
    NodeTwo[0].pNext = &NodeTwo[1];
    NodeTwo[1].iNumber =4;
    NodeTwo[1].pNext = &NodeTwo[2];
    NodeTwo[2].iNumber =5;
    NodeTwo[2].pNext = &NodeTwo[3];
    NodeTwo[3].iNumber =9;
    NodeTwo[3].pNext = &NodeTwo[4];
    NodeTwo[4].iNumber =7;
    NodeTwo[4].pNext = &NodeTwo[5];
    NodeTwo[5].iNumber =10;
    NodeTwo[5].pNext = NULL;
    DumpList(NodeOne);

    Sort(NodeOne);
    Sort(NodeTwo);
    printf("After Sort\n");

    pSearch = Merge(NodeOne, NodeTwo);

    DumpList(NodeOne);
    DumpList(pSearch);

    return 1;
}

