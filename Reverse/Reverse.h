#ifndef _REVERSE
#define _REVERSE

typedef struct _NODE *PNODE;

typedef struct _NODE
{
    CHAR character;
    PNODE pNext;
} NODE;

class cReverse
{
private:
    void ShoveSpace(PNODE pNewNode);
    void ShoveChar(PNODE pNewNode);
    PNODE CreateNode(char cNew);

public:
    cReverse();
    ~cReverse();
    bool AddChar(CHAR cNew);
    //DumpString(LPTSTR lpstrBuffer);
    void DumpString();

private:    // some private data
    NODE nHead;
    PNODE pTail;

};

#endif  // _REVERSE
