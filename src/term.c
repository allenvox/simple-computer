#include "term.h"

int mt_clrscr(void) // clears screen and moves cursor to upper left corner
{
  if (write (STDOUT_FILENO, CLEAR, strlen(CLEAR)) < sizeof(char) * strlen(CLEAR))
    {
      return -1;
    }
  return 0;
}

int mt_gotoXY(int x, int y) // moves cursor to the entered coordinates (x, y) = (row, col)
{

}

int mt_getscreensize(int *rows, int *cols) // gets terminal screen size (num of rows and cols)
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

int mt_setfgcolor(enum colors color) // sets a font color for upcoming characters
{

}

int mt_setbgcolor(enum colors color) // sets a background color for upcoming characters
{

}