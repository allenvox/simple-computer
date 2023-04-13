#include "readkey.h"

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
      *key = KEY_Q;
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
  tcsetattr (STDOUT_FILENO, TCSANOW, &current);
  memcpy (&backup, &current, sizeof (backup));
  return 0;
}

int
rk_mytermrestore ()
{
  return tcsetattr (STDOUT_FILENO, TCSANOW, &backup);
}

int
rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint)
{
	if (tcgetattr(STDOUT_FILENO, &current) != 0)
	  {
		  return -1;
	  }
	// канонический режим = 1
	if (regime)
	  {
      current.c_lflag |= ICANON;
    }
	else if (!regime)
	  {
		  current.c_lflag &= ~ICANON;
	  }
	else
	  {
		  return -1;
	  }
	// неканонический режим работы
  if (!regime)
    {
    	// количество символов в очереди, чтобы read завершился
      current.c_cc[VTIME] = vtime;
      // сколько времени ждать появления символа
      current.c_cc[VMIN] = vmin;
    }
  // символы будут отражаться по мере набора
  if (echo == 1)
    {
    	current.c_lflag |= ECHO;
    }
  else if (!echo)
    {
    	current.c_lflag &= ~ECHO;
   	}
  else
    {
      return -1;
    }
  // обработка клавиш прерывания
  if (sigint)
    {
      current.c_lflag |= ISIG;
    }
  else if (!sigint)
    {
		  current.c_lflag &= ~ISIG;
    }
  else
    {
      return -1;
    }
  return tcsetattr(STDOUT_FILENO, TCSANOW, &current);
}