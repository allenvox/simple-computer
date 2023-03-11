#include "bc.h"
#include "msc.h"
#include "readkey.h"
#include "term.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main ()
{
  mt_clrscr ();
  rk_mytermregime (0, 0, 1, 0, 1);
  sc_memoryInit ();
  for (int i = 3; i < 100; i += 3)
    {
      sc_memorySet (i, i * 3);
    }
  sc_regInit ();
  sc_regSet (1, 1);
  sc_regSet (2, 1);
  sc_regSet (4, 1);
  sc_regSet (16, 1);
  sc_accumSet (9999);
  sc_countSet (0);
  g_interface ();
  rk_mytermrestore ();
  return 0;
}