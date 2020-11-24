#include "queue.h"
#include "QueueWrapper.h" 
// Creates a FIFO byte queue, returning a handle to it
  cQueue * create_queue()
  {
      return new cQueue;
  }
 //void destroy_queue(Q* q); // Destroy a previously created byte queue
  // Adds a new byte to a queue <q>
  void enqueue_byte(cQueue* q, unsigned char b)
  {
      if (nullptr != q)
      {
          q->Add(b);
      }
 }
 //unsigned char dequeue_byte(Q* q);