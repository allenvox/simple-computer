#include "sig.h"
#include "cu.h"
#include "tui.h"

int
sig_handle_reset ()
{
  sc_memoryInit ();
  sc_regInit ();
  sc_regSet (FLAG_IGNORE, 1);
  alarm (0);
  return 0;
}

int
sig_handle_alarm ()
{
  CU ();
  int value;
  sc_countGet (&value);
  sc_countSet (value + 1);
  sc_countGet (&value);
  g_drawboxes ();
  int flag;
  int x = value / 10;
  int y = value % 10;
  g_highlightmemory (x, y);
  g_drawbcbox ();
  sc_regGet (FLAG_IGNORE, &flag);
  alarm (!flag);
  return 0;
}

int
sig_set ()
{
  signal (SIGUSR1, sig_handle_reset);
  signal (SIGALRM, sig_handle_alarm);
  return 0;
}