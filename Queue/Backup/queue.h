#ifndef _QUEUE_H
#define _QUEUE_H

class cQueue{
public:
    cQueue();
    ~cQueue();

    // Add cTile Element to the Queue
    bool Add(unsigned char);

    // Remove Element from the Queue
    unsigned char Remove();

    unsigned int SizeOfQ();

private:
    unsigned char m_bQueue[16];
    unsigned int m_bHead;
    unsigned int    m_bTail;
    unsigned int m_NumberOfElements;

};

#endif
