#include <stdio.h>
#include "queue.h"
#include "queuewrapper.h"

int main()
{

    cQueue * myQueue = create_queue();

    for (unsigned char counter = 0; counter < 16; counter++)
    {
        enqueue_byte(myQueue, counter);
    }
    return 1;
}

// Basic Can I enqueue an item
void TestOne()
{

}

// Basic can I create and destroy a queue
// Basic can I enqueue and dequeue an item

// Create two queues - destroying them in order
// Create two queues - destroy them in reverse order
// Can I create the maximum number of queues and destroy them
// Can I create the maximum number of queues, enqueue and dequeue items

// Error can I destroy a queue twice
// Error can I access a deleted queue

// Error code when I dequeue empty list
// Error code when I add item to list and dequeue twice
// Error code when I go past the end of the queue

