#ifndef _HEAP_H_
#define _HEAP_H_

#define HEAPSIZE 15

class cHeap
{
public:
    cHeap();
    ~cHeap();

    void Add(int iElement);

    void DumpHeap();

private:
    void AddFull(int iElement);
    void AddNotFull(int iElement);

    int iEnd;

    int iHeap[HEAPSIZE];

};

#endif // _HEAP_H_
