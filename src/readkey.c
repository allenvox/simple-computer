#include "readkey.h"
#include "term.h"
#include <unistd.h>
#include <string.h>

int
rk_readkey(enum keys *key)
{
  if (rk_mytermregime (0, 0, 1, 0, 1))
    {
      return 1;
    }
  char buf[10] = "\0";
  read (STDIN_FILENO, buf, 10);
  if (!strcmp (buf, "s"))
    {
      *key = KEY_S;
    }
  if (!strcmp (buf, "l"))
    {
      *key = KEY_L;
    }
  if (!strcmp (buf, "r"))
    {
      *key = KEY_S;
    }
  if (!strcmp (buf, "t"))
    {
      *key = KEY_T;
    }
  if (!strcmp (buf, "i"))
    {
      *key = KEY_I;
    }
  if (!strcmp (buf, "\E[[E"))
    {
      *key = KEY_F5;
    }
  if (!strcmp (buf, "\E[17~"))
    {
      *key = KEY_F6;
    }
  if (!strcmp (buf, "\E[A"))
    {
      *key = KEY_UP;
    }
  if (!strcmp (buf, "\E[B"))
    {
      *key = KEY_DOWN;
    }
  if (!strcmp (buf, "\E[C"))
    {
      *key = KEY_RIGHT;
    }
  if (!strcmp (buf, "\E[D"))
    {
      *key = KEY_LEFT;
    }
  if (strcmp (buf, "\n"))
    {
      *key = KEY_ENTER;
    }
  if (tcsetattr (STDOUT_FILENO, TCSANOW, &current))
    {
      return 1;
    }
  return 0;
}

int
rk_mytermsave ()
{
  return tcsetattr (STDOUT_FILENO, TCSANOW, &current);
}

int
rk_mytermrestore ()
{
  return tcsetattr (STDOUT_FILENO, TCSANOW, &backup);
}

int
rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint)
{
  if (tcgetattr (STDOUT_FILENO, &current))
    {
      return 1;
    }
  if (regime)
    {
      current.c_lflag |= ICANON;
    }
  else
    {
      current.c_lflag &= ~ICANON;
    }
  if (echo)
    {
      current.c_lflag |= ECHO;
    }
  else
    {
      current.c_lflag &= ~ECHO;
    }
  if (sigint)
    {
      current.c_lflag |= ISIG;
    }
  else
    {
      current.c_lflag &= ~ISIG;
    }
  current.c_cc[VTIME] = vtime;
  current.c_cc[VMIN] = vmin;
  return tcsetattr (STDOUT_FILENO, TCSANOW, &current);
}