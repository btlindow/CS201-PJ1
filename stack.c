#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

/*
 * Stack C File
 * --------------------
 *
 * File that contains all the functions to operate a stack
 * with value objects. Pushes and Pops the stack in constant time.
 *
 */

static void printPop(value*);
static void printPush(value*);

int printOption = 0;

/*
 * Function: initStack
 * --------------------
 *
 * Allocates space for the stack and initlizes stack values to NULL
 *
 */

Stack* initStack()
{
  Stack *s = malloc(sizeof(Stack));
  if (s == 0) { fprintf(stderr,"out of memory"); exit(-1); }
  s->head = NULL;
  return s;
}

/*
 * Function: push
 * --------------------
 *
 * Pushes a value object onto the top of the stack.
 *
 */

void push(value* v, Stack* s)
{
  Node* n = createNode(v);
  if(printOption) printPush(v);

  if (s->head == NULL)
    s->head = n;
  else
  {
    n->next = s->head;
    s->head = n;
  }
}

/*
 * Function: pop
 * --------------------
 *
 * Pops a value off the top of the stack.
 *
 */

value* pop(Stack* s)
{
  if(s->head == NULL)
  {
    return NULL;
  }

  Node* n = s->head;
  if (s->head->next == NULL)
    s->head = NULL;
  else
    s->head = s->head->next;
  if(printOption) printPop(n->val);
  return n->val;
}

/*
 * Function: printPop
 * --------------------
 *
 * An optional print function to show what is being popped off the stack.
 *
 */

static void printPop(value* v)
{
  printf("POPPED: ");
  if(v->type == INTEGER)
    printf("%d", v->ival);
  else if(v->type == REAL)
    printf("%f", v->rval);
  else
    printf("%s", v->sval);

  printf("\n");
}

/*
 * Function: printPush
 * --------------------
 *
 * An optional print function to show what is being pushed on the stack.
 *
 */
static void printPush(value* v)
{
  printf("PUSHED: ");
  if(v->type == INTEGER)
    printf("%d", v->ival);
  else if(v->type == REAL)
    printf("%f", v->rval);
  else
    printf("%s", v->sval);

  printf("\n");
}
