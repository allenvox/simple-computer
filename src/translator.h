#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
  char data;
  struct node* next;
} node;

void stack_push (char data, node** top);
void stack_print (node* top);
char stack_pop (node** top);
char stack_top (node* top);
int check_priority (char sign);
char* translate (char* inf, char rpn[]);