#include "readkey.h"
#include "term.h"
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios current, backup;

int
rk_readkey (enum keys *key)
{
	char buf[16];
	int numRead;
	if (tcgetattr(STDOUT_FILENO, &backup) != 0)
	  {
		  return -1;
	  }
	if (rk_mytermregime(0, 0, 1, 0, 1) != 0)
	  {
		  return -1;
	  }
	numRead = read(STDOUT_FILENO, buf, 15);
	buf[numRead] = '\0';
	if (strcmp(buf, "l") == 0)
	  {
		  *key = KEY_L;
	  }
	else if (strcmp(buf, "s") == 0)
	  {
		  *key = KEY_S;
	  }
	else if (strcmp(buf, "r") == 0)
	  {
		  *key = KEY_R;
	  }
  else if (strcmp(buf, "q") == 0)
	  {
		  *key = KEY_ESCAPE;
	  }
	else if (strcmp(buf, "t") == 0)
	  {
		  *key = KEY_T;
	  }
	else if (strcmp(buf, "i") == 0)
	  {
		  *key = KEY_I;
	  }
	else if ((strcmp(buf, "\n") == 0) || (strcmp(buf, "\r") == 0))
	  {
		  *key = KEY_ENTER;
	  }
	else if ((strcmp(buf, "\E[15~") == 0) || (strcmp(buf, "\E[[E") == 0))
	  {
		  *key = KEY_F5;
	  }
	else if (strcmp(buf, "\E[17~") == 0)
	  {
		  *key = KEY_F6;
	  }
	else if (strcmp(buf, "\E[A") == 0)
	  {
		  *key = KEY_UP;
	  }
	else if (strcmp(buf, "\E[B") == 0)
	  {
		  *key = KEY_DOWN;
	  }
	else if (strcmp(buf, "\E[C") == 0)
	  {
		  *key = KEY_RIGHT;
	  }
	else if (strcmp(buf, "\E[D") == 0)
	  {
		  *key = KEY_LEFT;
	  }
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &backup) != 0)
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