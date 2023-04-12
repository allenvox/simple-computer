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

  int cmd;
  sc_commandEncode(10, 78, &cmd); // READ 78
  sc_memorySet (3, cmd);
	sc_commandEncode(10, 88, &cmd); // READ 88
  sc_memorySet (6, cmd);
	sc_commandEncode(20, 79, &cmd); // LOAD 79
  sc_memorySet (9, cmd);
	sc_commandEncode(30, 89, &cmd); // ADD 89
  sc_memorySet (12, cmd);
	sc_commandEncode(21, 99, &cmd); // STORE 99
  sc_memorySet (15, cmd);
	sc_commandEncode(20, 78, &cmd); // LOAD 78
  sc_memorySet (18, cmd);
	sc_commandEncode(30, 88, &cmd); // ADD 88
  sc_memorySet (21, cmd);
	sc_commandEncode(21, 98, &cmd); // STORE 98
  sc_memorySet (24, cmd);
	sc_commandEncode(43, 0, &cmd); // HALT
  sc_memorySet (27, cmd);

  sc_regInit ();
  sc_regSet (FLAG_IGNORE, 1);
  sc_accumSet (9999);
  sc_countSet (0);
  sig_set ();

  rk_mytermsave ();
  rk_mytermregime (0, 0, 1, 0, 1);
  mt_clrscr ();
  g_interface ();
  rk_mytermrestore ();
  return 0;
}