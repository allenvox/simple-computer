#include "msc.h"
#include "term_gui.h"

void out_N_hor (int n)
{
  for (int i = 0; i < n; i++)
    {
      printf ("─");
    }
}

void out_border_top ()
{
  printf("┌");
  out_N_hor (32);
  printf (" Memory ");
  out_N_hor (32);
  printf ("┐ ┌");
  out_N_hor (6);
  printf (" Flags ");
  out_N_hor (6);
  printf ("┐\n");
}

void out_flags ()
{
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
}

void out_memcell (int n)
{
  int val;
  char cell[5];
  sc_memoryGet (n, &val);
  if (val < 0)
    {
      printf("-");
      val *= -1;
    }
  else
    {
      printf("+");
    }
  sprintf (cell, "%04d", val);
  printf ("%s", cell);
}

void out_GUI ()
{
  out_border_top ();
  for (int i = 0; i < 100; i++)
    {
      if (i == 0)
        {
          printf ("│  ");
        }
      out_memcell (i);
      printf("  ");
      if (i % 10 == 9)
        {
          printf ("│ ");
          if (i == 9)
            {
              printf ("│   ");
              out_flags ();
              printf ("   │");
            }
          if (i == 19)
            {
              printf ("└");
              out_N_hor (19);
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
  out_N_hor (10);
  printf (" Keys ");
  out_N_hor (10);
  printf ("┬");
  out_N_hor (45);
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
  out_N_hor (26);
  printf ("┴");
  out_N_hor (45);
  printf ("┘\n");
}