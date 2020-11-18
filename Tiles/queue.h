#ifndef _QUEUE_H
#define _QUEUE_H

class cQueue{
public:
    cQueue();
    ~cQueue();

    // Add cTile Element to the Queue
    STDMETHODIMP Add(BYTE);

    // Remove Element from the Queue
    STDMETHODIMP_(BYTE) Remove();

    STDMETHODIMP_(BYTE) SizeOfQ();

private:
    BYTE m_bQueue[16];
    BYTE m_bHead;
    BYTE m_bTail;
    BYTE m_NumberOfElements;

};

#endif
