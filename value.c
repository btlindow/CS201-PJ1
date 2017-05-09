#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "value.h"

char *INTEGER = "INTEGER";
char *REAL = "REAL";
char *STRING = "STRING";
char *VARIABLE = "VARIABLE";
char *OPERATOR = "OPERATOR";
char *SEMICOLON = "SEMICOLON";

static value *newValue(char *);
static int opValue(char *);

/*************** public interface *************/

value *
newIntegerValue(int v)
    {
    value *n = newValue(INTEGER);
    n->ival = v;
    return n;
    }

value *
newRealValue(double v)
    {
    value *n = newValue(REAL);
    n->rval = v;
    return n;
    }

value *
newStringValue(char *v)
    {
    value *n = newValue(STRING);
    n->sval = v;
    return n;
    }

value *
newVariableValue(char *v, char *t)
    {
    value *n = newValue(VARIABLE);
    n->sval = v;

    if (strchr(t,'.') != 0) // dot found!
    {
      n->iflag = 0;
      n->rval = atof(t);
    }
    else
    {
      n->iflag = 1;
      n->ival = atoi(t);
    }

    return n;
    }

value *
newOperatorValue(char *v)
    {
    value *n = newValue(OPERATOR);
    n->sval = v;
    n->ival = opValue(n->sval);
    return n;
    }

value *
newSemicolonValue(char *v)
    {
    value *n = newValue(SEMICOLON);
    n->sval = v;
    return n;
    }

void
displayValue(FILE *fp,value *v)
    {
    if (v->type == STRING)
       fprintf(fp,"\"%s\"",v->sval);
    else if (v->type == INTEGER)
       fprintf(fp,"%d",v->ival);
    else if (v->type == REAL)
       fprintf(fp,"%f",v->rval);
    else if (v->type == VARIABLE)
       fprintf(fp,"%s",v->sval);
    else if (v->type == OPERATOR)
       fprintf(fp,"%s",v->sval);
    else
       fprintf(fp,"<UNKNOWN VALUE TYPE>");
    }

/*************** private methods *************/

value *
newValue(char *t)
    {
    value *n = malloc(sizeof(value));
    if (n == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    n->type = t;
    return n;
    }

static int opValue(char *v)
{
  char o = v[0];
  switch (o)
  {
    case '+':
      return 1;
    case '-':
      return 2;
    case '*':
      return 3;
    case '/':
      return 4;
    case '%':
      return 5;
    case '^':
      return 6;
    case '=':
      return 0;
  }
  return -1;
}
