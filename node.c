#include <stdio.h>
#include <stdlib.h>

#include "node.h"

/*
 * Node C File
 * --------------------
 *
 * Function for allocating space for a Node object.
 *
 */

Node* createNode(value* v)
{
  Node *n = malloc(sizeof(Node));
  if (n == 0) { fprintf(stderr,"out of memory"); exit(-1); }
  n->val = v;
  n->next = NULL;
  n->left = NULL;
  n->right = NULL;
  return n;
}
