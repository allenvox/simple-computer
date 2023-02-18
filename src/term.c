#include "term.h"

int
mt_clrscr (void) // clears screen and moves cursor to upper left corner
{
  if (write (STDOUT_FILENO, CLEAR, strlen (CLEAR))
      < sizeof (char) * strlen (CLEAR))
    {
      return -1;
    }
  return 0;
}

int
mt_gotoXY (
    int x,
    int y) // moves cursor to the entered coordinates (x, y) = (row, col)
{
  char go[30];
  sprintf (go, "\E[%d;%dH", x, y);
  if (write (STDOUT_FILENO, go, strlen (go)) < sizeof (char) * strlen (go))
    {
      return -1;
    }
  return 0;
}

int
mt_getscreensize (
    int *rows, int *cols) // gets terminal screen size (num of rows and cols)
{
  struct winsize ws;
  if (ioctl (1, TIOCGWINSZ, &ws))
    {
      return -1;
    }
  *rows = ws.ws_row;
  *cols = ws.ws_col;
  return 0;
}

int
mt_setfgcolor (enum colors color) // sets a background color for all rows and
{                                 // cols, entire terminal
  char foreground[30];
  sprintf (foreground, "\E[38;5;%dm", color);
  if (write (STDOUT_FILENO, foreground, strlen (foreground))
      < sizeof (char) * strlen (foreground))
    {
      return -1;
    }
  return 0;
}

int
mt_setbgcolor (
    enum colors color) // sets a background color only for upcoming characters
{
  char background[30];
  sprintf (background, "\E[48;5;%dm", color);
  if (write (STDOUT_FILENO, background, strlen (background))
      < sizeof (char) * strlen (background))
    {
      return -1;
    }
  return 0;
}