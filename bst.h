#ifndef BST_H
#define BST_H

#include "node.h"
#include "value.h"

/*
 * BST Class Header File
 * ---------------------
 *
 * Defines BST Struct and Public Functions for BST
 *
 */

typedef struct BST
    {
      Node* root;

    } BST;

BST* initBST();
void addNode(value*, BST*);
void updateNode(char*, value*, BST*);
value* pullNode(char*, BST*);
void printBST(BST*);

#endif
