#include "tui.h"
#include "bc.h"
#include "msc.h"
#include "readkey.h"
#include "term.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int
g_flags (char **val)
{
  int flag = 0;
  sc_regGet (1, &flag);
  char F = flag == 1 ? 'F' : ' ';
  sc_regGet (2, &flag);
  char D = flag == 1 ? 'D' : ' ';
  sc_regGet (4, &flag);
  char A = flag == 1 ? 'A' : ' ';
  sc_regGet (8, &flag);
  char O = flag == 1 ? 'O' : ' ';
  sc_regGet (16, &flag);
  char C = flag == 1 ? 'C' : ' ';
  char buff[14];
  sprintf (buff, "%c  %c  %c  %c  %c", F, D, A, O, C);
  *val = buff;
  return 0;
}

int
g_static (void)
{
  bc_box (1, 1, 12, 63);
  mt_gotoXY (0, 28);
  write (STDOUT_FILENO, " Memory ", 8 * sizeof (char));
  bc_box (1, 64, 3, 39);
  mt_gotoXY (1, 77);
  write (STDOUT_FILENO, " accumulator ", 13 * sizeof (char));
  bc_box (4, 64, 3, 39);
  mt_gotoXY (4, 73);
  write (STDOUT_FILENO, " instructionCounter ", 20 * sizeof (char));
  bc_box (7, 64, 3, 39);
  mt_gotoXY (7, 78);
  write (STDOUT_FILENO, " Operation ", 11 * sizeof (char));
  bc_box (10, 64, 3, 39);
  mt_gotoXY (10, 79);
  write (STDOUT_FILENO, " Flags ", 7 * sizeof (char));
  bc_box (13, 1, 12, 63);
  bc_box (13, 64, 12, 39);
  mt_gotoXY (13, 67);
  write (STDOUT_FILENO, " Keys ", 7 * sizeof (char));
  char *str[7] = { "l - load",
                   "s - save",
                   "r - run",
                   "t - step",
                   "i - reset",
                   "F5 - accumulator",
                   "F6 - instructionCounter" };
  for (int i = 0; i < 7; i++)
    {
      mt_gotoXY (15 + i, 66);
      write (STDOUT_FILENO, str[i], strlen (str[i]));
    }
  mt_gotoXY (33, 0);
  return 0;
}

int
g_memorybox (void)
{
  int k = 0;
  for (int i = X_START; i < 12; i++)
    {
      for (int j = Y_START; j < 63; j += Y_STEP)
        {
          mt_gotoXY (i, j);
          char buff[6];
          int val;
          sc_memoryGet (k++, &val);
          sprintf (buff, "+%04d", val);
          write (STDERR_FILENO, buff, 6 * sizeof (char));
        }
    }
  return 0;
}

int
g_accumbox (void)
{
  mt_gotoXY (2, 80);
  char buff[5];
  int val;
  sc_accumGet (&val);
  sprintf (buff, "%04d", val);
  write (STDOUT_FILENO, buff, 5 * sizeof (char));
  mt_gotoXY (33, 0);
  return 0;
}

int
g_counterbox (void)
{
  mt_gotoXY (5, 80);
  char buff[5];
  int val;
  sc_countGet (&val);
  sprintf (buff, "%04d", val);
  write (STDOUT_FILENO, buff, 5 * sizeof (char));
  mt_gotoXY (33, 0);
  return 0;
}

int
g_operationbox (void)
{
  mt_gotoXY (8, 79);
  write (STDOUT_FILENO, "+00 : 00", 8 * sizeof (char));
  mt_gotoXY (33, 0);
  return 0;
}

int
g_flagbox (void)
{
  mt_gotoXY (11, 79);
  char *val;
  g_flags (&val);
  write (STDOUT_FILENO, val, 14 * sizeof (char));
  mt_gotoXY (33, 0);
  return 0;
}

int
g_bcbox (int *big)
{
  int count = 0;
  int val = 0;
  sc_countGet (&count);
  sc_memoryGet (count, &val);
  int digit[2] = { big[2 * 16], big[2 * 16 + 1] };
  bc_printbigchar (digit, BC_X, BC_START, GREEN, GREY);
  for (int i = 3; i >= 0; i--)
    {
      int radix = (int)pow (10, i);
      int k = 0;
      switch ((val / radix) % 10)
        {
        case 0x0:
          k = 0;
          break;
        case 0x1:
          k = 1;
          break;
        case 0x2:
          k = 2;
          break;
        case 0x3:
          k = 3;
          break;
        case 0x4:
          k = 4;
          break;
        case 0x5:
          k = 5;
          break;
        case 0x6:
          k = 6;
          break;
        case 0x7:
          k = 7;
          break;
        case 0x8:
          k = 8;
          break;
        case 0x9:
          k = 9;
          break;
        case 0xA:
          k = 10;
          break;
        case 0xB:
          k = 11;
          break;
        case 0xC:
          k = 12;
          break;
        case 0xD:
          k = 13;
          break;
        case 0xE:
          k = 14;
          break;
        case 0xF:
          k = 15;
          break;
        default:
          k = 17;
          break;
        }
      int digit[2] = { big[2 * k], big[2 * k + 1] };
      bc_printbigchar (digit, BC_X, BC_START + (4 - i) * BC_STEP, GREEN, GREY);
    }
  mt_gotoXY (33, 0);
  return 0;
}

int
g_loadmemory (void)
{
  bc_box (6, 20, 3, 26);
  mt_gotoXY (6, 24);
  write (STDOUT_FILENO, " Load from \n", strlen (" Load from \n"));
  char buff[20][1];
  mt_gotoXY (7, 21);
  for (int i = 0; i < 20; i++)
    {
      mt_gotoXY (7, 21 + i);
      read (STDOUT_FILENO, buff[i], 1);
      if (buff[i][0] == '\n')
        {
          break;
        }
      mt_gotoXY (7, 21);
      write (STDERR_FILENO, buff, strlen (buff));
    }
  buff[strlen (buff) - 1][0] = '\0';
  if (sc_memoryLoad (buff) != 0)
    {
      bc_box (6, 20, 3, 26);
      mt_gotoXY (6, 24);
      mt_setbgcolor (RED);
      write (STDOUT_FILENO, " Fail! \n", strlen (" Fail! \n"));
    }
  else
    {
      bc_box (6, 20, 3, 26);
      mt_gotoXY (6, 24);
      mt_setbgcolor (GREEN);
      write (STDOUT_FILENO, " Success! \n", strlen (" Success! \n"));
    }
  mt_setbgcolor (GREY);
  read (STDOUT_FILENO, NULL, 1);
  g_static ();
  return 0;
}

int
g_savememory (void)
{
  bc_box (6, 20, 3, 26);
  mt_gotoXY (6, 24);
  write (STDOUT_FILENO, " Save to \n", strlen (" Save to \n"));
  char buff[20][1];
  mt_gotoXY (7, 21);
  for (int i = 0; i < 20; i++)
    {
      mt_gotoXY (7, 21 + i);
      read (STDOUT_FILENO, buff[i], 1);
      if (buff[i][0] == '\n')
        {
          break;
        }
      mt_gotoXY (7, 21);
      write (STDERR_FILENO, buff, strlen (buff));
    }
  buff[strlen (buff) - 1][0] = '\0';
  sc_memorySave (buff);
  bc_box (6, 20, 3, 26);
  mt_gotoXY (6, 24);
  mt_setbgcolor (GREEN);
  write (STDOUT_FILENO, " Success! \n", strlen (" Success! \n"));
  mt_setbgcolor (GREY);
  read (STDOUT_FILENO, NULL, 1);
  g_static ();
  return 0;
}

int
g_init (int *big)
{
  g_memorybox ();
  g_accumbox ();
  g_counterbox ();
  g_operationbox ();
  g_flagbox ();
  g_bcbox (big);
  return 0;
}

int
g_setmemory (int x, int y)
{
  bc_box (6, 20, 3, 26);
  mt_gotoXY (6, 24);
  write (STDOUT_FILENO, " Set memory value to \n",
         strlen (" Set memory value to \n"));
  char buff[5][1];
  mt_gotoXY (7, 21);
  for (int i = 0; i < 4; i++)
    {
      mt_gotoXY (7, 21 + i);
      read (STDOUT_FILENO, buff[i], 1);
      if (buff[i][0] == '\n')
        {
          break;
        }
      mt_gotoXY (7, 21);
      write (STDERR_FILENO, buff, strlen (buff));
    }
  buff[4][0] = '\0';
  int val = atoi (buff);
  sc_memorySet (x * 10 + y, val);
  g_static ();
  g_memorybox ();
  return 0;
}

int
g_printmem (int *x, int *y)
{
  g_memorybox ();
  *x %= 10;
  *y %= 10;
  if (*x == -1)
    {
      *x = 9;
    }
  if (*y == -1)
    {
      *y = 9;
    }
  mt_gotoXY (X_START + (*x), Y_START + (*y) * Y_STEP);
  char buff[6];
  int address = (*x) * 10 + (*y);
  sc_countSet (address);
  int val;
  sc_memoryGet (address, &val);
  sprintf (buff, "+%04d", val);
  mt_setbgcolor (BLUE);
  write (STDERR_FILENO, buff, 6 * sizeof (char));
  mt_setbgcolor (GREY);
  return 0;
}

int
g_interface (int *big)
{
  int exit = 0, x = 0, y = 0, cnt = 0;
  mt_setbgcolor (GREY);
  g_static ();
  while (!exit)
    {
      enum keys key = KEY_DEFAULT;
      g_init (big);
      g_memorybox ();
      g_printmem (&x, &y);
      g_counterbox ();
      g_bcbox (big);
      rk_readkey (&key);
      switch (key)
        {
        case KEY_ESCAPE:
          exit++;
          break;
        case KEY_UP:
          x--;
          break;
        case KEY_DOWN:
          x++;
          break;
        case KEY_LEFT:
          y--;
          break;
        case KEY_RIGHT:
          y++;
          break;
        case KEY_L:
          g_loadmemory ();
          break;
        case KEY_S:
          g_savememory ();
          break;
        case KEY_I:
          sc_memoryInit ();
          x = 0, y = 0;
          g_static ();
          break;
        case KEY_ENTER:
          g_setmemory (x, y);
          break;
        default:
          break;
        }
      if (cnt == 5)
        {
          g_static ();
          cnt = 0;
        }
      cnt++;
    }
  return 0;
}