#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*
 * Queue C File
 * --------------------
 *
 * Contains all of the functions for Queue Operations.
 *
 */

int pOption = 0;

static void printNQ(value*);

/*
 * Function: iniQueue
 * --------------------
 *
 * Allocates and initizlizes values for a Queue.
 *
 */

Queue* initQueue()
{
  Queue *q = malloc(sizeof(Queue));
  if (q == 0) { fprintf(stderr,"out of memory"); exit(-1); }
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
  return q;
}

/*
 * Function: nq
 * --------------------
 *
 * Enqueue Function to add Node to the front of the Queue.
 *
 */

void nq(value* v, Queue* q)
{
  Node* n = createNode(v);
  if (pOption) printNQ(v);

  if(q->head == NULL)
  {
    q->head = n;
    q->tail = n;
  }
  else
  {
    q->tail->next = n;
    q->tail = n;
  }
  q->size++;
}

/*
 * Function: dq
 * --------------------
 *
 * Dequeue function to take the first thing out of the Queue.
 *
 */

value* dq(Queue* q)
{
  if (q->head == NULL)
  {
    return NULL;
  }
  else
  {
    Node* ptr = q->head;
    if(q->head->next == NULL)
      q->head = NULL;
    else
      q->head = q->head->next;
    q->size--;
    return ptr->val;
  }
}

/*
 * Function: printQueue
 * --------------------
 *
 * Function that traverses the Queue and prints the queue.
 *
 */

void printQueue(Queue* q)
{
  if(!q->head)
  {
    return;
  }

  Node* n = q->head;
  value* v;

  while(n)
  {
    v = n->val;

    if (v->type == INTEGER)
      printf("%d ",v->ival);
    else if (v->type == REAL)
      printf("%f ",v->rval);
    else if(strcmp(v->sval, "(") != 0 && strcmp(v->sval, ")") != 0)
      printf("%s ",v->sval);

      n = n->next;
  }
  printf("\n");
}

/*
 * Function: freeQueue
 * --------------------
 *
 *  A function to reset the queue back to NULL
 * with size 0.
 *
 */

void freeQueue(Queue* q)
{
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
}

/*
 * Function: QtoQ
 * --------------------
 *
 * Copies elements from one queue to the other starting at a
 * specific Node.
 *
 */

void QtoQ(Node* ptr, Queue* to)
{
  Node* n = ptr;
  while(n)
  {
    nq(n->val, to);
    n = n->next;
  }
}

/*
 * Function: printNQ
 * --------------------
 *
 * And optional print function to print the value
 * as it is put into the queue.
 *
 */

static void printNQ(value* v)
{
  if (v->type == INTEGER)
    printf("NQ'd: %d\n", v->ival);
  else
    printf("NQ'd: %s\n", v->sval);
}
