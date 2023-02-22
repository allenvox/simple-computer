#include "msc.h"
#include "term.h"
#include "term_gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main ()
{
  mt_clrscr ();
  sc_memoryInit ();
  for (int i = 3; i < 100; i *= 3)
    {
      sc_memorySet (i, i);
    }
  sc_regInit ();
  sc_regSet (1, 1);
  sc_regSet (2, 1);
  sc_regSet (4, 1);
  sc_regSet (16, 1);
  sc_accumSet(312);
  sc_countSet(11);
  g_static ();
  int big[36];
  g_memorybox ();
  g_accumbox ();
  g_counterbox ();
  g_operationbox ();
  g_flagbox ();
  return 0;
}