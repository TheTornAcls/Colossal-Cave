#ifndef __HASH_H
#define __HASH_H

#define HASHVAL 197

typedef struct _NODE *PNODE;

typedef struct _NODE
{
    LPSTR lpstrString;
    PNODE pNext;
} NODE;

class cHash
{
public:
    cHash();
    ~cHash();
    BOOL Add(LPSTR lpstrNew);

    VOID DumpHash();

private:
    PNODE HashTable[HASHVAL];

    UINT ComputeHash(LPSTR lpstrNew);

};

#endif
