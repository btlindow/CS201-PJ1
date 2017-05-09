#ifndef NODE_H
#define NODE_H

/*
 * Node Header File
 * ---------------------
 *
 * Defines Node Struct for Linked Lists and BSTs
 *
 */

#include "value.h"

typedef struct Node
    {
      value* val;
      struct Node* left;
      struct Node* right;
      struct Node* next;


    } Node;

Node* createNode (value*);

#endif
