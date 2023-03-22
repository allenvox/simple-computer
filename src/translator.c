#include "translator.h"

void stack_push (char data, node** top)
{
  node* tmp = (node*)malloc (sizeof(node));
  tmp->data = data;
  tmp->next = *top;
  *top = tmp;
}

char stack_pop (node** top)
{
  node* tmp;
  char d;
  if (*top == NULL)
    {
      return -1;
    }
  else
    {
      tmp = *top;
      *top = tmp->next;
      d = tmp->data;
      free (tmp);
      return d;
    }
}

char stack_top (node* top)
{
  if (top != NULL)
    {
      return top->data;
    }
}

int checkPriority (char sign)
{
  switch (sign)
    {
    case '*':
    case '/':
      return 4;
    case '+':
    case '-':
      return 2;
    case '(':
    case ')':
      return 1;
    }
}

char* translate (char* inf, char* rpn)
{
  //char rpn[255] = "\0";
  node* root = NULL;
  int i = 0, j = 0;
  while (inf[i] != '\0' && inf[i] != '\n')
    {
      char x = inf[i];
      if ((x >= 'a' && x <= 'z') || x >= 'A' && x <= 'Z')
        {
          rpn[j] = x;
          j++;
          //printf("%s\n", rpn);
        }
      else if (x == '(')
        {
           stack_push (x, &root);
        }
      else if (x == ')')
        {
          while (stack_top (root) != '(')
           {
              char c = stack_pop (&root);
              if (c != 0)
                {
                  rpn[j] = c;
                  j++;
                }
           }
          stack_pop (&root);
        }
      else if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          while (root != NULL && checkPriority (root->data) >= checkPriority (x))
           {
              char c = stack_pop (&root);
              if (c != 0)
                {
                  rpn[j] = c;
                  j++;
                }
           }
           stack_push (x, &root);
        }
      else if (x != ' ')
        {
          //free(rpn);
          fprintf (stderr, "Wrong expression!\n");
          exit (EXIT_FAILURE);
        }
      i++;
    }
  while (root != NULL)
    {
      char c = stack_pop (&root);
      if (c != 0)
        {
          rpn[j] = c;
          j++;
        }
    }
  for (int k = 0; k < j; k++)
    {
      if (rpn[k] == '(' || rpn[k] == ')')
        {
          fprintf (stderr, "Check your expression!\n");
          exit (EXIT_FAILURE);
        }
    }
  rpn[j] = '\0';
  return rpn;
}