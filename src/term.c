#include "term.h"

int
mt_clrscr (void)
{
  if ((unsigned long)write (STDOUT_FILENO, CLEAR, strlen (CLEAR))
      < sizeof (char) * strlen (CLEAR))
    {
      return -1;
    }
  return 0;
}

int
mt_gotoXY (int x, int y)
{
  char go[30];
  sprintf (go, "\E[%d;%dH", x, y);
  if ((unsigned long)write (STDOUT_FILENO, go, strlen (go))
      < sizeof (char) * strlen (go))
    {
      return -1;
    }
  return 0;
}

int
mt_getscreensize (int *rows, int *cols)
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
mt_setfgcolor (enum colors color)
{
  char foreground[30];
  sprintf (foreground, "\E[38;5;%dm", color);
  if ((unsigned long)write (STDOUT_FILENO, foreground, strlen (foreground))
      < sizeof (char) * strlen (foreground))
    {
      return -1;
    }
  return 0;
}

int
mt_setbgcolor (enum colors color)
{
  char background[30];
  sprintf (background, "\E[48;5;%dm", color);
  if ((unsigned long)write (STDOUT_FILENO, background, strlen (background))
      < sizeof (char) * strlen (background))
    {
      return -1;
    }
  return 0;
}

int
mt_readtext (char *text, int size)
{
  int numRead = read (STDOUT_FILENO, text, size);
  text[numRead] = '\0';
  return 0;
}

int
mt_printtext (char *text)
{
  write (STDOUT_FILENO, text, strlen (text));
  return 0;
}