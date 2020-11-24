#include "stdafx.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include "Queue.h"
#include "Exception.h"

// Note Queue have create, destory, enqueue and dequeue, verify these functions.
//      Queue allocate and free memory - we want to make sure our memory stays healthy
//      Queue logic can involve "Off by One" errors. Test some of the conditions that might expose these.
//      Queues are used in different environments. Some with specific latency requirements.  Look at the latency for
//             enqueue/dequeue and create/destroy
//
// Test the basic Create and destroy Queues - Test01, Test01A, Test01B.
// Test02 A/B- destroy empty queue and destroy queue twice
// Test02C - Verify "Out of Memory" exception is thrown when the 65th Q is created.
// Test03 A/BTest the basic errors - dequeue twice and dequeue from empty queue
// NOTE - these two tests fail - the exception for illegal opperation does not throw.  

// Test03C - Overflow the buffer of a single Q - make sure the "Out of Memory" exception is thrown AND verify memory

// NOTE: the three previous of test cases, I overrode the error functions
// to throw execeptions.  

// Test04 - create, fill and delete Q - verify no memory corruption
// Test05 - create two q's, fill and delete the second, create second - verify no memory corruption
// Test06 - create max Q and fill memory - verify memory.  Delete Queues and Verify Memory
// Test07 - create max Q and fill memory by adding element to each Q then moving to next Q.  Delete Queues and Verify Memory
// Test08 - check latency of Q creation and destruction
// Test09 - check latency of Q creation and destruction with populated Q's
// Test10 - check latency of Q enqueuing an dequeing
// Test11 - create max Q, delete two Qs in middle and create Q - verify memory


// Call the Windows HeapValidate API to check for memory corruption
bool CheckMemory()
{
    return HeapValidate(GetProcessHeap(), 0, nullptr);
}

// Due to the static nature of the memory structure handling the Queue, we must not leak Queues.
// Before we leave a test function, we must release all queues created.

// Can we create and destroy the maximum number of Queues
void Test01()
{
    printf("Starting Test01\n");
    Q* TestQueues[64];
    for (unsigned int counter = 0; counter < 64; counter++)
    {
        TestQueues[counter] = create_queue();
        if (nullptr == TestQueues[counter])
        {
            printf("Test01: failed to create  %d Q\n", counter);
            return;
        }
    }

    for (unsigned int counter = 0; counter < 64; counter++)
    {
        destroy_queue(TestQueues[counter]);
    }
    printf("Test01 Done\n");
}

// verify illegal operation is a called when queue is destroyed twice
void Test02A()
{
    printf("Starting Test02A\n");
    Q* qTest = create_queue();
    destroy_queue(qTest);
    try {
      destroy_queue(qTest);
      // NOTE - if we get here then code attempted to destroy an invalid Q
      printf("Test02A FAILED\n");
    }
    catch (IllegalOperation)
    {
        // Success - caught the illegal operation
        printf("Test02A Passed\n");
    }

    printf("Test02A Done\n");
}

// Verify illegal operation is called when called on nullptr
void Test02B()
{
    printf("Starting Test02B\n");

    try {
        destroy_queue(nullptr);
        // NOTE - if we get here then code has dequeue from a NULL queue
        printf("Test02B FAILED\n");
    }
    catch (IllegalOperation)
    {
        // Success - caught the illegal operation
        printf("Test02B Passed\n");
    }
    printf("Test02B Done\n");
}

// Verify "Out of Memory" exception is thrown when the 65th Q is created.
void Test02C()
{
    printf("Starting Test02C\n");
    Q* TestQueues[65];

    try {
        // We now have the baseline
        for (unsigned int counter = 0; counter < 65; counter++)
        {
            TestQueues[counter] = create_queue();
        }
        printf("Test02C FAILED\n");
    }
    catch (OutOfMemory)
    {
        // Success - caught the illegal operation
        printf("Test02C Passed\n");
    }
    for (unsigned int counter = 0; counter < 64; counter++)
    {
        destroy_queue(TestQueues[counter]);
    }
    printf("Test02C Done\n");
}

// Verify illegal operation called when dequeuing from queue
// that no longer has elements
void Test03A()
{
    printf("Starting Test03A\n");
    Q* qTest = create_queue();
    enqueue_byte(qTest, (unsigned char)1);
    dequeue_byte(qTest);
    try {
        dequeue_byte(qTest);
        // NOTE - if we get here then code has dequeue from a NULL queue
        printf("Test03A FAILED\n");
    }
    catch (IllegalOperation)
    {
        // Success - caught the illegal operation
        printf("Test03A Passed\n");
    }
    catch (std::exception ex)
    {
        printf("Test03B Failed - Wrong Exception thrown.\n%s\n ", ex.what());
    }
    destroy_queue(qTest);
    printf("Test03A Done\n");
}
// Verify illegal operation called when dequeueing from empty queue
void Test03B()
{
    printf("Starting Test03B\n");
    Q* qTest = create_queue();

    // Dequeue from empty queue
    try {
        dequeue_byte(qTest);
        // NOTE - if we get here then code has dequeue from a NULL queue
        printf("Test03B FAILED\n");
    }
    catch (IllegalOperation)
    {
        // Success - caught the illegal operation
        printf("Test03B Passed\n");
    }
    catch (std::exception ex)
    {
        printf("Test03B Failed - Wrong Exception thrown.\n%s\n ", ex.what());
    }
    destroy_queue(qTest);
    printf("Test03B Done\n");
}

// Add too many elements to the Q to get a OutOfMemory exception
// Remember the Q can hold 1792 items so we should fail when we add the 1793rd item.
void Test03C()
{
    printf("Starting Test03C\n");
    Q* TestQ;
    TestQ = create_queue();

    try
    {
        unsigned char input = 0;
        for (unsigned int counter = 0; counter <= 1793; counter++)
        {
            enqueue_byte(TestQ, input);
            input = (input + 1) % 256;
        }
        // NOTE - if we get here then code has dequeue from a NULL queue
        printf("Test03C FAILED\n");
    }
    catch (OutOfMemory)
    {
        // Success - caught the illegal operation
        printf("Test03C Passed\n");
    }
    if (!CheckMemory())
    {
        printf("Test03C FAILED - Memory is corrupted.\n");
    }
    destroy_queue(TestQ);
    if (!CheckMemory())
    {
        printf("Test03C FAILED - Memory is corrupted.\n");
    }
    printf("Test03C Done\n");
}

// able to destroy a full queue AND then create a new one
void Test05()
{
    printf("Starting Test05\n");
    Q* TestQueues[2];
    TestQueues[0] = create_queue();
    TestQueues[1] = create_queue();
    for (unsigned char input = 0; input < 255; input++)
    {
        enqueue_byte(TestQueues[1], input);
    }
    destroy_queue(TestQueues[1]);
    TestQueues[1] = create_queue();
    
    if (!CheckMemory())
    {
        printf("Memory is corrupted.\n");
    }
    destroy_queue(TestQueues[0]);
    destroy_queue(TestQueues[1]);
    if (!CheckMemory())
    {
        printf("Memory is corrupted.\n");
    }
    printf("Test05 Done\n");
}

// Fill up each Q to the max
// moving to the next queue
// Verify memory is still is good shape
// Delete each Q without dequeuing items.
// Again Verifying memory
void Test07()
{
    printf("Starting Test07\n");
    // Look at line 114 in Queue.cpp
    // static const unsigned int max_queued_byte_count = remaining_space / sizeof(queued_byte); 
    // Which is 1792 (looked the value while debugging)
    int totalAvailableMemoryForQueue = 1792;

    Q* TestQueues[64];
    for (unsigned int counter = 0; counter < 64; counter++)
    {
        TestQueues[counter] = create_queue();
    }

    unsigned char data = 0;
    unsigned int currentQ = 0;

    for (int counter = 0; counter < totalAvailableMemoryForQueue; counter++)
    {
        enqueue_byte(TestQueues[currentQ], data);
        data = (data + 1) % 255;
        currentQ = (currentQ + 1) % 64;
    }

    // Check for memory leaks/memory corruption here
    if (!CheckMemory())
    {
        printf("Memory is corrupted.\n");
    }
    for (int i = 63; i >= 0; --i)
    {
        destroy_queue(TestQueues[i]);
    }
    if (!CheckMemory())
    {
        printf("Memory is corrupted.\n");
    }
    printf("Test07 Done\n");
}

void Test11()
{
    printf("Starting Test11\n");
    // Look at line 114 in Queue.cpp
    // static const unsigned int max_queued_byte_count = remaining_space / sizeof(queued_byte); 
    // Which is 1792 (looked the value while debugging)
    int totalAvailableMemoryForQueue = 1792;

    Q* TestQueues[64];
    for (unsigned int counter = 0; counter < 64; counter++)
    {
        TestQueues[counter] = create_queue();
    }

    // Destroy Two Q's in middle
    destroy_queue(TestQueues[31]);
    destroy_queue(TestQueues[32]);
    
    // Create two new Q's
    TestQueues[31] = create_queue();
    TestQueues[32] = create_queue();

    // Check for memory leaks/memory corruption here
    if (!CheckMemory())
    {
        printf("Test11 FAILED - Memory is corrupted.\n");
    }
    for (int i = 63; i >= 0; --i)
    {
        destroy_queue(TestQueues[i]);
    }
    if (!CheckMemory())
    {
        printf("Test11 FAILED - Memory is corrupted.\n");
    }
    printf("Test11 Done\n");
}

// Check performance - using a single create/enqueue/dequeue/delete
// Then check performance for creating Q's
void Test08()
{
    printf("Starting Test08\n");
    LARGE_INTEGER liStart;
    LARGE_INTEGER liEnd;
    QueryPerformanceCounter(&liStart);
    Q* singleQ = create_queue();
    QueryPerformanceCounter(&liEnd);
    DWORD  slowestCreate = liEnd.LowPart - liStart.LowPart;
    DWORD  fastestCreate = slowestCreate;

    QueryPerformanceCounter(&liStart);
    destroy_queue(singleQ);
    QueryPerformanceCounter(&liEnd);
    DWORD  slowestDestroy = liEnd.LowPart - liStart.LowPart;
    DWORD  fastestDestroy = slowestDestroy;

    DWORD fastCreate = 65;
    DWORD fastDelete = 65;
    DWORD slowCreate = 65;
    DWORD slowDelete = 65;

    // We now have the baseline
    Q* TestQueues[64];
    for (unsigned int counter = 0; counter < 64; counter++)
    {
        QueryPerformanceCounter(&liStart);
        TestQueues[counter] = create_queue();
        QueryPerformanceCounter(&liEnd);

        if ((liEnd.LowPart - liStart.LowPart) > slowestCreate)
        {
            slowestCreate = liEnd.LowPart - liStart.LowPart;
            slowCreate = counter;
        }
        if ((liEnd.LowPart - liStart.LowPart) < fastestCreate)
        {
            fastestCreate = liEnd.LowPart - liStart.LowPart;
            fastCreate = counter;
        }
    }

    //Q* FailingQueue = create_queue(); - this will fail a queue too many
    for (unsigned int counter = 0; counter < 64; counter++)
    {
        QueryPerformanceCounter(&liStart);
        destroy_queue(TestQueues[counter]);
        QueryPerformanceCounter(&liEnd);

        if ((liEnd.LowPart - liStart.LowPart) > slowestDestroy)
        {
            slowestDestroy = liEnd.LowPart - liStart.LowPart;
            slowDelete = counter;
        }
        if ((liEnd.LowPart - liStart.LowPart) < fastestDestroy)
        {
            fastestDestroy = liEnd.LowPart - liStart.LowPart;
            fastCreate = counter;
        }
    }

    printf("Fastest Create is %d at %d\n", fastestCreate, fastCreate);
    printf("Slowest Create is %d at %d\n", slowestCreate, slowCreate);
    printf("Fastest Destroy is %d at %d\n", fastestDestroy, fastDelete);
    printf("Slowest Destroy is %d at %d\n", slowestDestroy, slowDelete);

    printf("Test08 Done\n");
}

// Check performance - using a single create/enqueue/dequeue/delete
// Then check performance for creating Q's
// Determine the min and max time to create a Q
// Determine the min and max time to desroy a Q
// With each queue Adding 20 items
// Use QueryPerformanceCounter to measure time in microseconds (GetTickCount did not have enough granularity)
void Test09()
{
    printf("Starting Test09\n");
    Q* firstQ = create_queue();
    // Add 20 characters to each queue
    for (unsigned char input = 0; input < 20; input++)
    {
        enqueue_byte(firstQ, input);
    }

    // We have a queue with 20 elements in place
    // create another queue and measure the time.
    LARGE_INTEGER liStart;
    LARGE_INTEGER liEnd;
    QueryPerformanceCounter(&liStart);
    Q* secondQ = create_queue();
    QueryPerformanceCounter(&liEnd);
    DWORD  slowestCreate = liEnd.LowPart - liStart.LowPart;
    DWORD  fastestCreate = slowestCreate;
    // We don't need to measure the deletion of this queue as it is empty
    destroy_queue(secondQ);

    // We want to measure the deletion of the first queue because it has items enqueued
    QueryPerformanceCounter(&liStart);
    destroy_queue(firstQ);
    QueryPerformanceCounter(&liEnd);
    DWORD  slowestDestroy = liEnd.LowPart - liStart.LowPart;
    DWORD  fastestDestroy = slowestDestroy;

    printf("Baseline Create is %d\n", fastestCreate);
    printf("Baseline Delete is %d\n", fastestDestroy);

    DWORD fastCreate = 65;
    DWORD fastDelete = 65;
    DWORD slowCreate = 65;
    DWORD slowDelete = 65;

    Q* TestQueues[64];

    // We now have the baseline
    // We need to have a queue create with items already enqueued
    firstQ = create_queue();
    // Add 20 characters to each queue
    for (unsigned char input = 0; input < 20; input++)
    {
        enqueue_byte(firstQ, input);
    }

    // Wo we are comparing apples to apples
    for (unsigned int counter = 0; counter < 63; counter++)
    {
        QueryPerformanceCounter(&liStart);
        TestQueues[counter] = create_queue();
        QueryPerformanceCounter(&liEnd);

        if ((liEnd.LowPart - liStart.LowPart) > slowestCreate)
        {
            slowestCreate = liEnd.LowPart - liStart.LowPart;
            slowCreate = counter;
        }
        if ((liEnd.LowPart - liStart.LowPart) < fastestCreate)
        {
            fastestCreate = liEnd.LowPart - liStart.LowPart;
            fastCreate = counter;
        }

        // Add 20 characters to each queue
        for (unsigned char input = 0; input < 20; input++)
        {
            enqueue_byte(TestQueues[counter], input);
        }
    }

    for (unsigned int counter = 0; counter < 63; counter++)
    {
        QueryPerformanceCounter(&liStart);
        destroy_queue(TestQueues[counter]);
        QueryPerformanceCounter(&liEnd);

        if ((liEnd.LowPart - liStart.LowPart) > slowestDestroy)
        {
            slowestDestroy = liEnd.LowPart - liStart.LowPart;
            slowDelete = counter;
        }
        if ((liEnd.LowPart - liStart.LowPart) < fastestDestroy)
        {
            fastestDestroy = liEnd.LowPart - liStart.LowPart;
            fastCreate = counter;
        }
    }
    // Delete that first queue not created in the loop.
    destroy_queue(firstQ);

    printf("Fastest Create is %d at %d\n", fastestCreate, fastCreate);
    printf("Slowest Create is %d at %d\n", slowestCreate, slowCreate);
    printf("Fastest Destroy is %d at %d\n", fastestDestroy, fastDelete);
    printf("Slowest Destroy is %d at %d\n", slowestDestroy, slowDelete);

    printf("Test09 Done\n");
}

// Add an element to the each queue (measure the time to enqueue_byte)
// Move to the next queue to add - eventually adding the maximum amount
// Dequeue the head from each queue, moving the next queue
// Determine the max and min time to enqueue byte.
// Determine the max and min time to dequeue byte.
void Test10()
{
    printf("Starting Test10\n");
    // Look at line 114 in Queue.cpp
    // static const unsigned int max_queued_byte_count = remaining_space / sizeof(queued_byte); 
    // Which is 1792 (looked the value while debugging)
    int totalAvailableMemoryForQueue = 1792;

    // We now have the baseline
    // We need to have a queue create with items already enqueued
    Q* firstQ = create_queue();
    // Enqueue and element

    LARGE_INTEGER liStart;
    LARGE_INTEGER liEnd;
    QueryPerformanceCounter(&liStart);
    enqueue_byte(firstQ, 255);
    QueryPerformanceCounter(&liEnd);
    DWORD  slowestEnqueue = liEnd.LowPart - liStart.LowPart;
    DWORD  fastestEnqueue = slowestEnqueue;

    // We want to measure the deletion of the first queue because it has items enqueued
    QueryPerformanceCounter(&liStart);
    dequeue_byte(firstQ);
    QueryPerformanceCounter(&liEnd);
    DWORD  slowestDequeue = liEnd.LowPart - liStart.LowPart;
    DWORD  fastestDequeue = slowestDequeue;

    destroy_queue(firstQ);

    printf("Baseline Enqueue is %d\n", fastestEnqueue);
    printf("Baseline Dequeue is %d\n", fastestDequeue);

    DWORD fastEnqueueLocation = totalAvailableMemoryForQueue;
    DWORD fastDequeueLocation = totalAvailableMemoryForQueue;
    DWORD slowEnqueueLocation = totalAvailableMemoryForQueue;
    DWORD slowDequeueLocation = totalAvailableMemoryForQueue;

    Q* TestQueues[64];
    for (unsigned int counter = 0; counter < 64; counter++)
    {
        TestQueues[counter] = create_queue();
    }

    unsigned char data = 0;
    unsigned int currentQ = 0;

    for (int counter = 0; counter < totalAvailableMemoryForQueue; counter++)
    {
        QueryPerformanceCounter(&liStart);
        enqueue_byte(TestQueues[currentQ], data);
        QueryPerformanceCounter(&liEnd);
        if ((liEnd.LowPart - liStart.LowPart) > slowestEnqueue)
        {
            slowestEnqueue = liEnd.LowPart - liStart.LowPart;
            slowEnqueueLocation = counter;
        }
        if ((liEnd.LowPart - liStart.LowPart) < fastestEnqueue)
        {
            fastestEnqueue = liEnd.LowPart - liStart.LowPart;
            fastEnqueueLocation = counter;
        }

        data = (data + 1) % 255;
        currentQ = (currentQ + 1) % 64;
    }

    // Now dequeue all of the items
    currentQ = 0;
    for (int counter = 0; counter < totalAvailableMemoryForQueue; counter++)
    {
        QueryPerformanceCounter(&liStart);
        dequeue_byte(TestQueues[currentQ]);
        QueryPerformanceCounter(&liEnd);
        if ((liEnd.LowPart - liStart.LowPart) > slowestDequeue)
        {
            slowestDequeue = liEnd.LowPart - liStart.LowPart;
            slowDequeueLocation = counter;
        }
        if ((liEnd.LowPart - liStart.LowPart) < fastestDequeue)
        {
            fastestDequeue = liEnd.LowPart - liStart.LowPart;
            fastDequeueLocation = counter;
        }

        currentQ = (currentQ + 1) % 64;
    }

    // Check for memory leaks/memory corruption here
    if (!CheckMemory())
    {
        printf("Memory is corrupted.\n");
    }
    for (int i = 63; i >= 0; --i)
    {
        destroy_queue(TestQueues[i]);
    }
    if (!CheckMemory())
    {
        printf("Memory is corrupted.\n");
    }

    printf("Fastest Enqueue is %d at %d\n", fastestEnqueue, fastEnqueueLocation);
    printf("Slowest Enqueue is %d at %d\n", slowestEnqueue, slowEnqueueLocation);
    printf("Fastest Dequeue is %d at %d\n", fastestDequeue, fastDequeueLocation);
    printf("Slowest Dequeue is %d at %d\n", slowestDequeue, slowDequeueLocation);

    printf("Test10 Done\n");
}

int main(int argc, const char* argv[])
{
    Test01();
    Test01A();
    Test01B();
    Test02A();
    Test02B();
    Test02C();
    Test03A();
    Test03B();
    Test03C();
    Test04();
    Test05();
    Test06();
    Test07();
    Test08();
    Test09();
    Test10();
    Test11();

    return 0;
}