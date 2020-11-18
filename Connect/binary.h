#ifndef _BINARY_H
#define _BINARY_H

typedef struct _CONNECT *CONNECTPTR;

typedef struct _CONNECT
{
    CHAR cData;
    CONNECTPTR pLeft;
    CONNECTPTR pRight;
} CONNECT;

class cBinary
{
public:
    cBinary();
    ~cBinary();
    void AddItem(CHAR cChar);
    void PrintLevel();
    void PrintChildren();
	bool ValidateTree();

private:
    void TreeDelete(CONNECTPTR);
    void FindChildren(cStructure *, CONNECTPTR);
    void FindChildrenStack(cStructure *, CONNECTPTR);
	bool PrivateValidateTree(CONNECTPTR);
    void virtual Identification();
    CONNECTPTR m_pHead;
    
protected:
    CONNECTPTR NodeCreate(CHAR cChar);
    void virtual Descend(CONNECTPTR, CONNECTPTR);

};

#endif //_BINARY_H
