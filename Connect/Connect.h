#ifndef _CONNECT_H
#define _CONNECT_H

class cBinTree:public cBinary
{
public:
    cBinTree();
    ~cBinTree();
    
    void ChildPrint();

private:
    void Identification();
    void Descend(CONNECTPTR, CONNECTPTR);
};


#endif  // _CONNECT_H
