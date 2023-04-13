#include "bc.h"
#include "msc.h"
#include "readkey.h"
#include "sig.h"
#include "term.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main ()
{
  sc_memoryInit ();
  sc_memorySet (1, 313);
  sc_memorySet (11, 0x7fff);
  sc_memorySet (21, 0x3fff);
  sc_memorySet (31, 0x851); // read 81
  sc_memorySet (41, 0x8d1); // write 81

  sc_regInit ();
  sc_regSet (FLAG_IGNORE, 1);
  sc_accumSet (0);
  sc_countSet (0);
  sig_set ();

  rk_mytermsave ();
  rk_mytermregime (0, 0, 1, 0, 1);
  mt_clrscr ();
  g_interface ();
  rk_mytermrestore ();
  return 0;
}