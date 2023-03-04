#include "readkey.h"
#include "term.h"
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios current, backup;

int
rk_readkey (enum keys *key)
{
  char buff[16];
  int numRead;
  if (tcgetattr (STDOUT_FILENO, &backup) != 0)
    {
	  return -1;
    }
  if (rk_mytermregime (0, 0, 1, 0, 1) != 0)
	{
	  return -1;
	}
  numRead = read (STDOUT_FILENO, buff, 15);
  buff[numRead] = '\0';
  if (strcmp (buff, "l") == 0)
	{
	  *key = KEY_L;
	}
  else if (strcmp (buff, "s") == 0)
	{
	  *key = KEY_S;
	}
  else if (strcmp (buff, "r") == 0)
    {
	  *key = KEY_R;
    }
  else if (strcmp (buff, "q") == 0)
	{
	  *key = KEY_ESCAPE;
	}
  else if (strcmp (buff, "t") == 0)
	{
		*key = KEY_T;
	}
  else if (strcmp (buff, "i") == 0)
	{
	  *key = KEY_I;
	}
  else if ((strcmp (buff, "\n") == 0) || (strcmp (buff, "\r") == 0))
	{
	  *key = KEY_ENTER;
	}
  else if ((strcmp (buff, "\E[15~") == 0) || (strcmp (buff, "\E[[E") == 0))
    {
	  *key = KEY_F5;
	}
  else if (strcmp (buff, "\E[17~") == 0)
    {
	  *key = KEY_F6;
	}
  else if (strcmp (buff, "\E[A") == 0)
    {
	  *key = KEY_UP;
    }
  else if (strcmp (buff, "\E[B") == 0)
	{
	  *key = KEY_DOWN;
	}
  else if (strcmp (buff, "\E[C") == 0)
	{
	  *key = KEY_RIGHT;
	}
  else if (strcmp (buff, "\E[D") == 0)
	{
	  *key = KEY_LEFT;
	}
  if (tcsetattr (STDOUT_FILENO, TCSANOW, &backup) != 0)
    {
	  return -1;
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