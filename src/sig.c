#include "sig.h"
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

int sig_handle_alarm ()
{
  g_drawboxes ();
  int value;
  sc_countGet (&value);
  sc_countSet (value + 1);
  int x = (value + 1) / 10;
  int y = (value + 1) % 10;
  g_highlightmemory (x, y);
  g_drawbcbox ();
  alarm (1);
  sc_regSet (FLAG_IGNORE, 0);
  return 0;
}

int
sig_set ()
{
  signal (SIGUSR1, sig_handle_reset);
  signal (SIGALRM, sig_handle_alarm);
  return 0;
}