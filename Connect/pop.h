#ifndef _POP_H
#define _POP_H

typedef struct _NODE *NODEPTR;

typedef struct _NODE
{
    NODEPTR pNext;
    PVOID pLeaf;
} NODE;

class cStructure
{
public:
    cStructure();
    ~cStructure();

    BOOL IsEmpty();
    virtual void Push(PVOID) = 0;
    PVOID Pop();

protected:
    NODEPTR m_pHead;
    NODEPTR m_pTail;
};

class cQueue : public cStructure
{
public:
    cQueue();
    ~cQueue();

    void Push(PVOID);

};

class cStack : public cStructure
{
public:
    cStack();
    ~cStack();

    void Push(PVOID);

};

#endif //_POP_H
