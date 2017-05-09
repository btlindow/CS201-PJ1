#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"
#include "value.h"

/*
 * Queue Header File
 * ---------------------
 *
 * Defines Queue Struct for Holding Values
 * Contains a head and tail pointer. Also contains INT size of the queue.
 * Enqueues and Dequeues in constant time.
 *
 */

typedef struct Queue
    {
      Node* head;
      Node* tail;

      int size;

    } Queue;

Queue* initQueue();
void nq(value*, Queue*);
value* dq(Queue*);
void printQueue(Queue*);
void freeQueue(Queue*);
void QtoQ(Node*, Queue*);

#endif
