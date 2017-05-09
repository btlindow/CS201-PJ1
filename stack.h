#ifndef STACK_H
#define STACK_H

#include "node.h"
#include "value.h"

/*
 * Stack Header File
 * ---------------------
 *
 * Defines the struct for a stack. Contains a head node.
 *
 */

typedef struct Stack
    {
      Node* head;

    } Stack;


Stack* initStack();
void push(value*, Stack*);
value* pop(Stack*);

#endif
