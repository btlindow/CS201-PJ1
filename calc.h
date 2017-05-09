#ifndef CALC_H
#define CALC_H

#include "value.h"
#include "stack.h"
#include "queue.h"
#include "node.h"
#include "bst.h"

/*
 * BST Class Header File
 * ---------------------
 *
 * Public Function for postFixCalculate
 *
 */

value* postfixCalculate(Queue*, BST*);

#endif
