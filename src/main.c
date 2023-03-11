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
  for (int i = 3; i < 100; i += 3)
    {
      sc_memorySet (i, i * 3);
    }
  sc_regInit ();
  sc_regSet (FLAG_IGNORE, 1);
  sc_accumSet (9999);
  sc_countSet (0);
  sig_setSignals ();

  rk_mytermsave ();
  rk_mytermregime (0, 0, 1, 0, 1);
  mt_clrscr ();
  g_interface ();
  rk_mytermrestore ();
  return 0;
}