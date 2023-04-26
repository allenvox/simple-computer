// RPN (Reverse Polish Notation) - logical operations form, where operands are
// located before operations stack machine - algorithm that performs
// calculations using RPN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
  char data;
  struct node *next;
} node;

void errOutput (char *message);

// stack machine functions
void stack_push (char data, node **top);
void stack_print (node *top);
char stack_pop (node **top);
char stack_top (node *top);

// RPN functions
int check_priority (char sign);
char *translateToRPN (char *inf, char rpn[]);