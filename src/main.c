#include "msc.h"
#include "term.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int output_memreg ()
{
  printf("┌");
  for (int i = 0; i < 6 * 5 + 2; i++)
    {
      printf ("─");
    }
  printf (" Memory ");
  for (int i = 0; i < 6 * 5 + 2; i++)
    {
      printf ("─");
    }
  printf ("┐ ┌");
  for (int i = 0; i < 6; i++)
    {
      printf ("─");
    }
  printf (" Flags ");
  for (int i = 0; i < 6; i++)
    {
      printf ("─");
    }
  printf ("┐\n");
  for (int i = 0; i < 100; i++)
    {
      if (i == 0)
        {
          printf ("│  ");
        }
      int val;
      char cell[5];
      sc_memoryGet (i, &val);
      if (val > 0)
        {
          printf("-");
          val *= -1;
        }
      else
        {
          printf("+");
        }
      sprintf (cell, "%04d", val);
      printf ("%s  ", cell);
      if (i % 10 == 9)
        {
          printf ("│ ");
          if (i == 9)
            {
              printf ("│   ");
              int flag = 0;
              sc_regGet(1, &flag);
              char F = flag == 1 ? 'F' : ' ';
              sc_regGet(2, &flag);
              char D = flag == 1 ? 'D' : ' ';
              sc_regGet(4, &flag);
              char A = flag == 1 ? 'A' : ' ';
              sc_regGet(8, &flag);
              char O = flag == 1 ? 'O' : ' ';
              sc_regGet(16, &flag);
              char C = flag == 1 ? 'C' : ' ';
              printf ("%c  %c  %c  %c  %c", F, D, A, O, C);
              printf ("   │");
            }
          if (i == 19)
            {
              printf ("└");
              for (int j = 0; j < 6 * 3 + 1; j++)
                {
                  printf ("─");
                }
              printf ("┘");
            }
          printf ("\n");
          if (i < 99)
            {
              printf("│  ");
            }
        }
    }
  printf ("├");
  for (int i = 0; i < 10; i++)
    {
      printf ("─");
    }

  printf (" Keys ");
  for (int i = 0; i < 10; i++)
    {
      printf ("─");
    }
  printf ("┬");
  for (int i = 0; i < 6 * 7 + 3; i++)
    {
      printf ("─");
    }
  printf ("┤\n│ ");
  printf ("l — load\ts - save   ");
  printf ("│\n│ ");
  printf ("r — run\tt - step   ");
  printf ("│\n│ ");
  printf ("i — reset\tF5 - accum ");
  printf ("│\n│ ");
  printf ("F6 — instruction counter ");
  printf ("│\n");
  printf ("└");
  for (int i = 0; i < 6 * 4; i++)
    {
      printf ("─");
    }
  printf ("\n");
  return 0;
}

int
main ()
{
  mt_clrscr ();
  sc_memoryInit ();
  sc_regInit ();
  sc_regSet(1, 1);
  sc_regSet(2, 1);
  sc_regSet(4, 1);
  sc_regSet(16, 1);
  output_memreg ();
  return 0;
}