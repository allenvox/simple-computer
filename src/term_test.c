#include "term.h"
#include <stdio.h>

int
term_test ()
{
  mt_clrscr ();
  printf ("hello, this is default terminal\n");
  getchar ();

  mt_clrscr ();
  mt_setbgcolor (GREEN);
  printf ("hello, this is green background for outputted text\n");
  getchar ();

  mt_setbgcolor (DEFAULT);
  mt_clrscr ();
  mt_setfgcolor (RED);
  printf ("hello, this is red background for everything\n");
  getchar ();

  mt_clrscr ();
  return 0;
}