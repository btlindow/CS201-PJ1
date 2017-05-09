#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"

/*
 * BST C File
 * -------------------------
 *
 * BSTs hold Nodes with with value objects.
 *
 */

static void inOrder(Node*);

/*
 * Function:  initBst
 * --------------------
 *
 * Allocates memory for the BST.
 * Initializes root to NULL.
 *
 * Returns a BST pointer.
 */

BST* initBST()
{
  BST *b = malloc(sizeof(BST));
  if (b == 0) { fprintf(stderr,"out of memory"); exit(-1); }
  b->root=NULL;
  return b;
}

/*
 * Function:  addNode
 * --------------------
 *
 * Adds a Node to the BST.
 * Inherently updates Node if same name is added.
 *
 */

void addNode(value* v, BST* b)
{
  if (b->root == NULL)
  {
    b->root = createNode(v);
    return;
  }

  Node* ptr = b->root;

  while(1)
  {
    if (strcmp(v->sval, ptr->val->sval) < 0 && ptr->left != NULL)
      ptr = ptr->left;
    else if (strcmp(v->sval, ptr->val->sval) < 0 && ptr->left == NULL)
    {
      ptr->left = createNode(v);
      return;
    }

    if (strcmp(v->sval, ptr->val->sval) > 0 && ptr->right != NULL)
      ptr = ptr->right;
    else if (strcmp(v->sval, ptr->val->sval) > 0 && ptr->right == NULL)
    {
      ptr->right = createNode(v);
      return;
    }

    if (strcmp(v->sval, ptr->val->sval) == 0)
    {
      ptr->val = v;
      return;
    }
  }
}

/*
 * Function:  updateNode
 * --------------------
 *
 * Finds an existing element in the search tree and
 * updates the value of that element.
 *
 */

void updateNode(char* c, value* knew, BST* b)
{
  Node* ptr = b->root;

  while(1)
  {
    while (strcmp(c, ptr->val->sval) < 0 && ptr->left != NULL)
      ptr = ptr->left;

    while (strcmp(c, ptr->val->sval) > 0 && ptr->right != NULL)
      ptr = ptr->right;

    if (strcmp(c, ptr->val->sval) == 0)
    {
      if(knew->type == INTEGER)
      {
        ptr->val->ival = knew->ival;
        ptr->val->iflag = 1;
      }
      else if(knew->type == REAL)
      {
        ptr->val->rval = knew->rval;
        ptr->val->iflag = 0;
      }
      else
      {
        ptr->val->varsval = knew->sval;
        ptr->val->iflag = 2;
      }

      return;
    }
  }
}

/*
 * Function:  pullNode
 * --------------------
 *
 * Searches the BST for given string value.
 *
 * Returns pointer to Node if found.
 * Returns NULL if not found.
 */

value* pullNode(char* v, BST* b)
{
  Node* ptr = b->root;

  if(b->root == NULL || v == NULL) return NULL;

  while(1)
  {
    if (strcmp(v, ptr->val->sval) < 0 && ptr->left != NULL)
      ptr = ptr->left;
    else if (strcmp(v, ptr->val->sval) < 0 && ptr->left == NULL)
      return NULL;

    if (strcmp(v, ptr->val->sval) > 0 && ptr->right != NULL)
      ptr = ptr->right;
    else if (strcmp(v, ptr->val->sval) > 0 && ptr->right == NULL)
      return NULL;

    if (strcmp(v, ptr->val->sval) == 0)
      return ptr->val;
  }
}

/*
 * Function:  printBST
 * --------------------
 *
 * Starts the recursive print function.
 *
 */

void printBST(BST* b)
{
  printf("BST\n---\n");
  inOrder(b->root);
}

//******Private Methods*********//

/*
 * Function:  inOrder
 * --------------------
 *
 * Recursive function that prints a BST via in-order
 * traversal.
 *
 */

void inOrder(Node* b)
{
  Node* n = b;

  if(n == NULL)
    return;

  if (n->left)
    inOrder(n->left);

    printf("%s: ",n->val->sval);
    if (n->val->iflag == 0)
      {
        printf("%f\n",n->val->rval);
      }
    else if (n->val->iflag == 1)
      {
        printf("%d\n",n->val->ival);
      }
    else
      {
        printf("%s\n",n->val->varsval);
      }

  if (n->right)
    inOrder(n->right);
}
