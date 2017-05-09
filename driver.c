#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "value.h"
#include "node.h"
#include "stack.h"
#include "queue.h"
#include "bst.h"
#include "convert.h"
#include "calc.h"

/*
*   DRIVER C FILE FOR CALCULON BY BEN LINDOW
*   ----------------------------------------
*   [CREDITS]
*   Credit to Dr. Lusth (University of Alabama) for support files and functions
*   used in this project.
*
*   [DESCRIPTION]
*   CALCULON is a program that can take arithmatic line expressions and
*   tranform the valid infix expression to postfix OR evaluate the expression
*   and print a resulting answer.
*
*   [OPTIONS]
*             calculon -v               Print Authors Name
*             calculon -d               User Line by Line w/ Postfix on EOF
*             calculon -d [FILENAME]    File Line by Line w/ Postfix on EOF
*             calculon [FILENAME]       Fiile Line by Line w/ Result
*             calculon                  User Line by Line w/ Reuslt
*/

//Globals
FILE *fp;
BST *b;
Stack *s;
value* result;
Queue *ifixQ;
Queue *pfixQ;
Queue *line;
Queue *lastP;

//Local Functions
void ProcessOptions(int,char **);
void readFromFile();
void readFromFileD(char*);
void readFromUser();
void readFromUserD();
int getQueue(Queue*);
void theCalc(int);
static value *readValue(FILE *);
static void printValue(value *);

/*
 * Function: main
 * --------------------
 *
 * Main driver for calculon. Initilizes global stacks and queues.
 *
 */

int main(int argc,char **argv)
  {
    b = initBST();
    s = initStack();
    ifixQ = initQueue();
    pfixQ = initQueue();
    line = initQueue();
    lastP = initQueue();

    ProcessOptions(argc,argv);

    return 0;
  }

  /*
   * Function: ProcessOptions
   * --------------------
   *
   * Uses Program Call variables and processes them
   * to determine appropriate file pointer for reading
   * input. Sets a DFLAG if the -d option is called.
   *
   */

void ProcessOptions(int argc, char **argv)
{

  switch (argc)
  {
    case 1:
      fp = stdin;
      theCalc(0);
      return;
    case 2:
      fp = fopen(&argv[1][0], "r");
      if(fp)
      {
        theCalc(0);
        return;
      }
      else if (!strcmp(argv[1], "-d"))
      {
        fp = stdin;
        theCalc(1);
        return;
      }
      else if (!strcmp(argv[1], "-v"))
      {
        printf("Benjamin T. Lindow\n");
        return;
      }
      else
      {
        printf("Could Not Process Option\n");
        exit(1);
      }
      return;
    case 3:
      fp = fopen(argv[2], "r");
      if (fp)
        theCalc(1);
      else
        printf("Could Not Open File\n");
      return;
    default:
      printf("TOO MANY OPTIONS\n");
      exit(1);
    }
}

/*
 * Function: theCalc
 * --------------------
 *
 * Function to evaluate the line currently in the line queue.
 * Formats the proper infix expression to be passed to the postfix
 * conversion function.
 *
 */

void theCalc(int dflag)
{
  value *v;

  while(getQueue(line))
  {
    freeQueue(pfixQ);
    freeQueue(ifixQ);

    if(line->head->val->type == STRING && pullNode(line->head->val->sval, b))
    {
      v = pullNode(line->head->val->sval, b);
      if(v)
      {
        if(line->head->next->val->type == OPERATOR)
        {
          if (strcmp(line->head->next->val->sval, "=") == 0)
          {
            Node* ptr = line->head->next->next;
            QtoQ(ptr, ifixQ);
            if(ifixQ->size > 2)
            {
              postfix(s, ifixQ, pfixQ);
              updateNode(v->sval, postfixCalculate(pfixQ, b), b);
              result = pullNode(v->sval, b);
            }
            else if (ifixQ->size == 2)
            {
              updateNode(v->sval, ifixQ->head->val, b);
              result = pullNode(v->sval, b);
            }
            else
            {
              printf("NOT VALID EXPRESSION!!!\n");
              exit (212);
            }
          }
          if (strcmp(line->head->next->val->sval, "=") != 0)
          {
            postfix(s, line, pfixQ);
            result = postfixCalculate(pfixQ, b);
          }
        }
        else if(line->head->next->val->type == SEMICOLON)
          result = v;
        else
        {
          printf("Not Valid Operation\n");
          exit (3);
        }
      }
    }
    else if(line->head->val->type == STRING && !pullNode(line->head->val->sval, b))
      {
        printf("Variable Not Declared\n");
        exit(2);
      }
    else if (line->head->next && strcmp(line->head->next->val->sval, "=") != 0)
    {
      postfix(s, line, pfixQ);
      result = postfixCalculate(pfixQ, b);
    }
  }
  printf("  \n");
  if(dflag)
    if(pfixQ->head)
      printQueue(pfixQ);
    else if (lastP->head)
      printQueue(lastP);
    else
      printf("No Expression To Postfix\n");
  else
    if(result)
      printValue(result);
    else
      printf("Could Not Find Value\n");
}

/*
 * Function: readValue
 * --------------------
 *
 * Reads a string token from the current file stream and processes
 * the string to determine which value object to create.
 *
 * Returns the value object created from the file stream.
 */

static value *readValue(FILE *fp)
{
  value *v = NULL;

  if (stringPending(fp))
      v = newStringValue(readString(fp)); //was stdin
   else
  {
    char *token = readToken(fp);
    char *p = token;
    p++;

    if(!token)
      return NULL;

    if (strchr(token,'.') != 0) // dot found!
      v = newRealValue(atof(token));
    else if ((strchr(token, '-') != 0 && isdigit(*p)) || isdigit(*token))
      v = newIntegerValue(atoi(token));
    else if (*token == ';')
      v = newSemicolonValue(token);
    else if (strcmp("var", token) == 0)
    {
      freeQueue(lastP);
      char* vname = readToken(fp);
      token = readToken(fp);
      v = newVariableValue(vname, "1");
      addNode(v, b);
      getQueue(ifixQ);
      if(ifixQ->size > 2)
      {
        postfix(s, ifixQ, pfixQ);
        QtoQ(pfixQ->head, lastP);
        value* t = postfixCalculate(pfixQ, b);
        updateNode(v->sval, t, b);
      }
      else
        updateNode(v->sval, ifixQ->head->val, b);

      result = v;
      value* semicol = ifixQ->tail->val;
      freeQueue(ifixQ);
      freeQueue(pfixQ);
      return semicol;
    }
    else if (*token == '+' || *token == '-' || *token == '*' || *token == '/' ||
             *token == '%' || *token == '^' || *token == ')' || *token == '(' ||
             *token == '=')
      v = newOperatorValue(token);
    else
    {
      v = newStringValue(token);
      v->iflag = 4;
    }
  }
return v;
}

/*
 * Function: printValue
 * --------------------
 *
 * Takes a value object and prints the appropraite string
 * representing the value object.
 *
 */

static void printValue(value *v)
{
  if (v->type == INTEGER)
    printf("%d\n",v->ival);
  else if (v->type == OPERATOR)
  {
    printf("%s\n",v->sval);
  }
  else if (v->type == SEMICOLON)
    printf("%s\n",v->sval);
  else if (v->type == REAL)
    printf("%f\n",v->rval);
  else if (v->type == VARIABLE)
  {
    if (v->iflag == 1)
      printf("%d\n",v->ival);
    else if(v->iflag == 0)
      printf("%f\n",v->rval);
    else if(v->iflag == 2)
      printf("\"%s\"\n", v->varsval);
  }
  else
    printf("\"%s\"\n",v->sval);
}

/*
 * Function: getQueue
 * --------------------
 *
 * A function that populates a queue with values until the current values
 * read is a semicolon.
 *
 * Returns 0 if a NULL value object is NULL from EOF.
 * Returns 1 if queue is populated correctly.
 */

int getQueue(Queue* theQ)
{
  value* v;
  freeQueue(theQ);

  do {
    v = readValue(fp);
    if (!v)
      return 0;
    nq(v, theQ);
  } while(v->type != SEMICOLON);
  return 1;
}
