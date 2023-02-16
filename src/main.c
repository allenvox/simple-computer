#include "msc.h"
#include "term.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int output_memreg ()
{
  printf("|  ");
  for (int i = 0; i < 5 * 6 - 4; i++)
    {
      printf ("—");
    }
  printf (" Memory ");
  for (int i = 0; i < 5 * 6 - 4; i++)
    {
      printf ("—");
    }
  printf ("\n");
  for (int i = 0; i < 100; i++)
    {
      if (i == 0)
        {
          printf ("|  ");
        }
      int val;
      char cell[5];
      sc_memoryGet (i, &val);
      sprintf (cell, "%04d", val);
      printf ("%s  ", cell);
      if (i % 10 == 9)
        {
          printf ("|\n|  ");
        }
    }
  for (int i = 0; i < 5 * 6 - 5; i++)
    {
      printf ("—");
    }
  printf (" Register ");
  for (int i = 0; i < 5 * 6 - 5; i++)
    {
      printf ("—");
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
  output_memreg ();
  return 0;
}