#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "calc.h"

/*
 * Calc C File
 * --------------------
 *
 * Contains all of the functions for determining which
 * arithmatic operations need to be done on two objects
 * as well as perfoming the aritmatic.
 *
 */

/********PRIVATE FUNCTION PROTOTYPES********/

static int howTo(value*, value*);
static value* intAndInt(value*, value*, value*);
static value* intAndReal(value*, value*, value*);
static value* realAndInt(value*, value*, value*);
static value* realAndReal(value*, value*, value*);
static value* stringAndString(value*, value*);
static value* stringAndReal(value*, value*, value*, BST* tree);
static value* realAndString(value*, value*, value*, BST* tree);
static value* stringAndInt(value*, value*, value*, BST* tree);
static value* intAndString(value*, value*, value*, BST* tree);
static value* calculate(value*, value*, value*, BST* tree);

/*
 * Function:  postFixCalculate
 * ---------------------------
 *
 * Inial function called to perform operations a
 * postfix queue. Uses a search tree to reference the
 * potentially used variables.
 *
 * Returns a value object of the resulting value.
 */

value* postfixCalculate(Queue* q, BST* tree)
{
  Stack* s = initStack();
  Node* ptr = q->head;
  value* O;
  value* a;
  value* b;
  value* result;

  while(ptr)
  {
    O = ptr->val;

    if(O->type == OPERATOR)
    {
      a = pop(s);
      b = pop(s);
      result = calculate(b, O, a, tree);
      push(result, s);
    }
    else
      push(O, s);

    ptr = ptr->next;
  }
  return s->head->val;
}

/*
 * Function:  calculate
 * --------------------
 *
 * Determines which type of values are being used in
 * an operation and directs values to the right operations.
 *
 * Returns a value object of the resulting value on value operation.
 */

static value* calculate(value* a, value* O, value* b, BST* tree)
{
  value* var = pullNode(a->sval, tree);

  if(!var && a->iflag == 4)
  {
    printf("Variable Not Declared\n");
    exit(1);
  }

  if(a->type == STRING && var)
  {
    a->type = VARIABLE;
    a->iflag = var->iflag;
    if(a->iflag == 1)
      a->ival = var->ival;
    else if (a->iflag == 0)
      a->rval = var->rval;
    else
      a->varsval = var->varsval;
  }

  var = pullNode(b->sval, tree);

  if(!var && b->iflag == 4)
  {
    printf("Variable Not Declared\n");
    exit(1);
  }

  if(b->type == STRING && var)
  {
    b->type = VARIABLE;
    b->iflag = var->iflag;
    if(b->iflag == 1)
      b->ival = var->ival;
    else if (b->iflag == 0)
      b->rval = var->rval;
    else
      b->varsval = var->varsval;
  }


  if (a->type == VARIABLE && a->iflag == 1)
    a = newIntegerValue(a->ival);
  else if (a->type == VARIABLE && a->iflag == 0)
    a = newRealValue(a->rval);
  else if (a->type == VARIABLE && a->iflag == 2)
    a = newStringValue(a->varsval);

  if (b->type == VARIABLE && b->iflag == 1)
    b = newIntegerValue(b->ival);
  else if (b->type == VARIABLE && b->iflag == 0)
    b = newRealValue(b->rval);
  else if (b->type == VARIABLE && b->iflag == 2)
    b = newStringValue(b->varsval);

  value* v = NULL;
  int opt = howTo(a, b);

  switch (opt)
  {
    case 1:
      v = stringAndString(a, b);
      break;
    case 2:
      v = stringAndReal(a, O, b, tree);
      break;
    case 3:
      v = realAndString(a, O, b, tree);
      break;
    case 4:
      v = intAndString(a, O, b, tree);
      break;
    case 5:
      v = stringAndInt(a, O, b, tree);
      break;
    case 6:
      v = intAndInt(a, O, b);
      break;
    case 7:
      v = intAndReal(a, O, b);
      break;
    case 8:
      v = realAndInt(a, O, b);
      break;
    case 9:
      v = realAndReal(a, O, b);
      break;
    default:
      if (v == NULL)
        printf("INVALID INPUT\n");
      exit(-9);
  }
  return v;
}

/*
 * Function:  howTo
 * --------------------
 *
 * Takes two value objects and determines which type of
 * operation to perform on the objects.
 *
 * Returns an INT of the case to use.
 */


static int howTo(value* a, value *b)
{
       if(a->type == STRING && b->type == STRING)
    return 1;
  else if(a->type == STRING && b->type == REAL)
    return 2;
  else if(a->type == REAL && b->type == STRING)
    return 3;
  else if(a->type == INTEGER && b->type == STRING)
    return 4;
  else if(a->type == STRING && b->type == INTEGER)
    return 5;
  else if(a->type == INTEGER && b->type == INTEGER)
    return 6;
  else if(a->type == INTEGER && b->type == REAL)
    return 7;
  else if(a->type == REAL && b->type == INTEGER)
    return 8;
  else if(a->type == REAL && b->type == REAL)
    return 9;

  return -1;
}

/*
 * Function:  intAndInt
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* intAndInt(value* aval, value* O, value* bval)
{
  value* v;
  int a = aval->ival;
  int b = bval->ival;
  int c = -1;

  if (strcmp(O->sval, "+") == 0)
    c = a + b;
  else if (strcmp(O->sval, "-") == 0)
    c = a - b;
  else if (strcmp(O->sval, "*") == 0)
    c = a * b;
  else if (strcmp(O->sval, "/") == 0)
    c = a / b;
  else if (strcmp(O->sval, "%") == 0)
    c = a % b;
  else if (strcmp(O->sval, "^") == 0)
    c = (int) pow((double)a, (double)b);


  return v = newIntegerValue(c);
}

/*
 * Function:  realAndReal
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* realAndReal(value* aval, value* O, value* bval)
{
  value* v;
  double a = aval->rval;
  double b = bval->rval;
  double c = -1;

  if (strcmp(O->sval, "+") == 0)
    c = a + b;
  else if (strcmp(O->sval, "-") == 0)
    c = a - b;
  else if (strcmp(O->sval, "*") == 0)
    c = a * b;
  else if (strcmp(O->sval, "/") == 0)
    c = a / b;
  else if (strcmp(O->sval, "%") == 0)
    c = (int) a % (int) b;
  else if (strcmp(O->sval, "^") == 0)
    c = pow(a, b);

  return v = newRealValue(c);
}

/*
 * Function:  stringAndString
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* stringAndString(value* aval, value* bval)
{
  value* v;

  char *c = malloc(sizeof(char) * (strlen(aval->sval) + strlen(bval->sval) + 1));
  if (c == 0) printf("out of memory\n");
  sprintf(c,"%s%s",aval->sval,bval->sval);
  return v = newStringValue(c);
}

/*
 * Function:  stringAndReal
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* stringAndReal(value* aval, value* O, value* bval, BST* tree)
{
  value* a2;
  char* token = aval->sval;
  a2 = newRealValue(atof(token));

  return calculate(a2, O, bval, tree);
}

/*
 * Function:  realAndString
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* realAndString(value* aval, value* O, value* bval, BST* tree)
{
  value* b2;
  char* token = bval->sval;
  b2 = newRealValue(atof(token));

  return calculate(aval, O, b2, tree);
}

/*
 * Function:  stringAndInt
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* stringAndInt(value* aval, value* O, value* bval, BST* tree)
{
  value* a2;
  char* token = aval->sval;
  a2 = newIntegerValue(atoi(token));

  return calculate(a2, O, bval, tree);
}

/*
 * Function:  intAndString
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* intAndString(value* aval, value* O, value* bval, BST* tree)
{
  value* b2;
  char* token = bval->sval;
  b2 = newIntegerValue(atoi(token));

  return calculate(aval, O, b2, tree);
}

/*
 * Function:  intAndReal
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* intAndReal(value* aval, value* O, value* bval)
{
  value* v;
  int a = aval->ival;
  double b = bval->rval;
  double c = -1;

  if (strcmp(O->sval, "+") == 0)
    c = a + b;
  else if (strcmp(O->sval, "-") == 0)
    c = a - b;
  else if (strcmp(O->sval, "*") == 0)
    c = a * b;
  else if (strcmp(O->sval, "/") == 0)
    c = a / b;
  else if (strcmp(O->sval, "%") == 0)
    c = (int) a % (int) b;
  else if (strcmp(O->sval, "^") == 0)
    c = pow((double)a, (double)b);

  return v = newRealValue(c);
}

/*
 * Function:  realAndInt
 * --------------------
 *
 * One case function for two value types.
 *
 * Returns a value object of the two resulting values.
 */

static value* realAndInt(value* aval, value* O, value* bval)
{
  value* v;
  double a = aval->rval;
  int b = bval->ival;
  double c = -1.0;

  if (strcmp(O->sval, "+") == 0)
    c = a + b;
  else if (strcmp(O->sval, "-") == 0)
    c = a - b;
  else if (strcmp(O->sval, "*") == 0)
    c = a * b;
  else if (strcmp(O->sval, "/") == 0)
    c = a / b;
  else if (strcmp(O->sval, "%") == 0)
    c = (int) a % (int) b;
  else if (strcmp(O->sval, "^") == 0)
    c = pow(a, (double)b);

  return v = newRealValue(c);
}
