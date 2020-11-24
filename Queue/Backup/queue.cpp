#include "queue.h"
// Queue Constructor           
cQueue::cQueue()
{
    m_bTail = 0;
    m_bHead = 0;
    m_NumberOfElements = 0;
    return;
}

// cQueue DeConstructor
cQueue::~cQueue()
{
    return;
}

// Add cTile Element to the Queue
bool  cQueue::Add(unsigned char bTileElement)
{
    m_bQueue[m_bTail] = bTileElement;
    m_bTail = (m_bTail + 1) % 16;

    // Increase the number of elements in the queue
    m_NumberOfElements++;


    return true;
}

unsigned char cQueue::Remove()
{
    if (m_NumberOfElements)
    {
        unsigned char bReturn;

        bReturn = m_bQueue[m_bHead];

        // Adjust the head number
        m_bHead = (m_bHead + 1) % 16;

        // Decrease the number of elements in the queue
        m_NumberOfElements--;

        return bReturn;

    }
    else
    {
        return -1;
    }

}

// return the size of the Q
unsigned int cQueue::SizeOfQ()
{
    return m_NumberOfElements;
}
