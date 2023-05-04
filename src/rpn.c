#include "rpn.h"

void
errOutput (char *message)
{
  fprintf (stderr, "%s", message);
  exit (EXIT_FAILURE);
}

void
stack_push (char data, node **top)
{
  node *tmp = (node *)malloc (sizeof (node));
  tmp->data = data;
  tmp->next = *top;
  *top = tmp;
}

char
stack_pop (node **top)
{
  node *tmp;
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

char
stack_top (node *top)
{
  return top->data;
}

int
checkPriority (char sign)
{
  switch (sign)
    {
    case '*':
    case '/':
      return 3;
    case '+':
    case '-':
      return 2;
    case '(':
    case ')':
      return 1;
    }
  return 0;
}

char *
translateToRPN (char *inf, char *rpn)
{
  node *root = NULL;
  int j = 0;
  for (int i = 0; inf[i] != '\0' && inf[i] != '\n'; i++)
    {
      char x = inf[i];

      // if current char is variable/constant - just add to RPN str
      if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
        {
          rpn[j] = x;
          j++;
        }

      // if open parentheses - push to stack
      else if (x == '(')
        {
          stack_push (x, &root);
        }

      // if closed parentheses - pop all operations till open parentheses
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

      // if operator - check its priority and add
      else if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          while (root != NULL
                 && checkPriority (root->data) >= checkPriority (x))
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

      // if neither var/const/operator/space - wrong character
      else if (x != ' ')
        {
          errOutput ("Wrong expression\n");
        }
    }

  // pop all operations and add them to RPN str
  while (root != NULL)
    {
      char c = stack_pop (&root);
      if (c != 0)
        {
          rpn[j] = c;
          j++;
        }
    }

  // if left any parentheses - user inputted too much of them
  for (int k = 0; k < j; k++)
    {
      if (rpn[k] == '(' || rpn[k] == ')')
        {
          errOutput ("Check your expression for unneeded parentheses\n");
        }
    }
  rpn[j] = '\0';
  return rpn;
}