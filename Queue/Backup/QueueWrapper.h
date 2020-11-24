 #ifndef _QUEUEWRAPPER_H
#define _QUEUEWRAPPER_H
 cQueue* create_queue(); // Creates a FIFO byte queue, returning a handle to it
 //void destroy_queue(Q* q); // Destroy a previously created byte queue
 void enqueue_byte(cQueue* q, unsigned char b); // Adds a new byte to a queue <q>
 //unsigned char dequeue_byte(Q* q); // Pops the next byte off the queue <q>
//If the functions run out of memory, it will call this function (which you may define / override):
 //void on_out_of_memory();
//If the caller makes an illegal request, it will call this function (which you may define / override):
 //void on_illegal_operation();
 #endif
