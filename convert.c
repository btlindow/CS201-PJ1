#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convert.h"

/*
 * Convert C File
 * --------------------
 *
 * Contains all of the functions for converting an
 * infix expression queue to a postfix expression
 * queue.
 *
 *
 */

static int isOperand(value*);
static int isOperator(value*);
static int isEmptyStack(Stack*);
static int isTopHigher(Stack*, int);
static int isSemicolon(value*);
static int isOpenParen(value*);
static int isCloseParen(value*);

/*
 * Function:  postfix
 * --------------------
 *
 * Main function for transforming an infix queue
 * into a postfix queue.
 *
 */

void postfix(Stack *s, Queue *ifixQ, Queue* pfixQ)
{
  Node* in = ifixQ->head;
  value* v;

  while(in)
  {
    v = in->val;

    if(isOperand(v))
      nq(v,pfixQ);

    else if (isOpenParen(v))
      push(v, s);

    else if(isCloseParen(v))
    {
      while(!isOpenParen(s->head->val) && !isEmptyStack(s))
        nq(pop(s), pfixQ);
      pop(s);
    }

    else if(isOperator(v) && !isOpenParen(v) && !isCloseParen(v))
    {
      if(!isEmptyStack(s))
      {
        while(!isEmptyStack(s) && isTopHigher(s, v->ival))
        {
          nq(pop(s), pfixQ);
        }
        push(v, s);
      }
      else if(isEmptyStack(s) || isOpenParen(s->head->val))
        push(v, s);
    }

    else if(isSemicolon(v))
    {
      while(!isEmptyStack(s))
      {
        nq(pop(s), pfixQ);
      }
      return;
    }
    in = in->next;
  }
}

/*
 * Function:  isOperand
 * --------------------
 *
 * Determins if a value type is an Operand.
 *
 * Returns 1 if the value is an Operand.
 * Returns 0 if the value is NOT an Operand.
 */

static int isOperand(value* v)
{
  if (v->type == INTEGER ||  v->type == REAL || v->type == VARIABLE || v->type == STRING)
    return 1;
  else
    return 0;
}

/*
 * Function:  isOperator
 * --------------------
 *
 * Determins if a value type is an Operator.
 *
 * Returns 1 if the value is an Operator.
 * Returns 0 if the value is NOT an Operator.
 */

static int isOperator(value* v)
{
  if (v->type == OPERATOR)
    return 1;
  else
    return 0;
}

/*
 * Function:  isEmptyStack
 * --------------------
 *
 * Determins if a stack is a empty.
 *
 * Returns 1 if the stack IS empty.
 * Returns 0 if the stack is NOT empty.
 */

static int isEmptyStack(Stack* s)
{
  if (s->head == NULL)
    return 1;
  else
    return 0;
}

/*
 * Function:  isTopHigher
 * --------------------
 *
 * Determins if the top of the stack's value object
 * contains a higher precedence
 *
 * Returns 1 if the top is higher precedence.
 * Returns 0 if the top is NOT higher precedence.
 */

static int isTopHigher(Stack* s, int in)
{
  int top = s->head->val->ival;

  if (top >=  in)
    return 1;
  else
    return 0;
}

/*
 * Function:  isSemiColon
 * --------------------
 *
 * Determins if a value type is a Semicolon.
 *
 * Returns 1 if the value is a SemiColon.
 * Returns 0 if the value is NOT a Semicolon.
 */

static int isSemicolon(value* v)
{
  if (v->type == SEMICOLON)
    return 1;
  else
    return 0;
}

/*
 * Function:  isOpenParen
 * --------------------
 *
 * Determins if a value type is an open parentheses.
 *
 * Returns 1 if the value is an open parentheses.
 * Returns 0 if the value is NOT an open parentheses.
 */

static int isOpenParen(value* v)
{
  if (strcmp(v->sval, "(") == 0)
    return 1;
  else
    return 0;
}

/*
 * Function:  isCloseParen
 * --------------------
 *
 * Determins if a value type is a closed parentheses.
 *
 * Returns 1 if the value is a closed parentheses.
 * Returns 0 if the value is NOT a closed parentheses.
 */

static int isCloseParen(value* v)
{
  if (strcmp(v->sval, ")") == 0)
    return 1;
  else
    return 0;
}
