#include "tui.h"

int
g_writeflags (char **val)
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
  sc_regGet (32, &flag);
  char I = flag == 1 ? 'I' : ' ';
  char buff[24] = { 0 };
  sprintf (buff, "%c  %c  %c  %c  %c  %c\0", F, D, A, O, C, I);
  *val = buff;
  return 0;
}

int
g_drawborders (void)
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
g_drawaccumbox (void)
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
g_drawflagbox (void)
{
  mt_gotoXY (11, 78);
  char *val;
  g_writeflags (&val);
  write (STDOUT_FILENO, val, 24 * sizeof (char));
  mt_gotoXY (33, 0);
  return 0;
}

int
g_drawbcbox ()
{
  int big[18][2] = {
    { 0x4242423C, 0x3C424242 }, // 0
    { 0x48506040, 0x40404040 }, // 1
    { 0x2042423C, 0x7E020418 }, // 2
    { 0x3840423C, 0x3C424040 }, // 3
    { 0x7E424478, 0x40404040 }, // 4
    { 0x3E02027E, 0x3C424040 }, // 5
    { 0x3E02423C, 0x3C424242 }, // 6
    { 0x2040407E, 0x10101010 }, // 7
    { 0x3C42423C, 0x3C424242 }, // 8
    { 0x7C42423C, 0x3C424040 }, // 9
    { 0x66663C18, 0x66667E7E }, // A
    { 0x3E66663E, 0x3E666666 }, // B
    { 0x0202423C, 0x3C420202 }, // C
    { 0x4444443E, 0x3E444444 }, // D
    { 0x3E02027E, 0x7E020202 }, // E
    { 0x1E02027E, 0x02020202 }, // F
    { 0x7E181800, 0x0018187E }, // +
    { 0x7E000000, 0x7E }        // -
  };
  int address, val;
  sc_countGet (&address);
  char buff[6];
  g_getunit (address, &buff);
  int *digit = malloc (2 * sizeof (int));
  for (int i = 0; i < 5; i++)
    {
      int k = 0;
      switch (buff[i])
        {
        case '0':
          k = 0;
          break;
        case '1':
          k = 1;
          break;
        case '2':
          k = 2;
          break;
        case '3':
          k = 3;
          break;
        case '4':
          k = 4;
          break;
        case '5':
          k = 5;
          break;
        case '6':
          k = 6;
          break;
        case '7':
          k = 7;
          break;
        case '8':
          k = 8;
          break;
        case '9':
          k = 9;
          break;
        case 'a':
          k = 10;
          break;
        case 'b':
          k = 11;
          break;
        case 'c':
          k = 12;
          break;
        case 'd':
          k = 13;
          break;
        case 'e':
          k = 14;
          break;
        case 'f':
          k = 15;
          break;
        case '+':
          k = 16;
          break;
        default:
          k = 17;
          break;
        }
      digit[0] = big[k][0];
      digit[1] = big[k][1];
      bc_printbigchar (digit, BC_X, BC_START + i * BC_STEP, GREEN, GREY);
    }
  mt_gotoXY (33, 0);
  return 0;
}

int
g_loadmemory (void)
{
  mt_gotoXY (25, 1);
  write (STDOUT_FILENO, "Load from file:>", strlen ("Load from file:>"));
  char buff[20][1];
  mt_gotoXY (25, 18);
  for (int i = 0; i < 20; i++)
    {
      mt_gotoXY (25, 18 + i);
      read (STDOUT_FILENO, buff[i], 1);
      if (buff[i][0] == '\n')
        {
          break;
        }
      mt_gotoXY (25, 18);
      write (STDERR_FILENO, buff, strlen (buff));
    }
  buff[strlen (buff) - 1][0] = '\0';
  mt_gotoXY (26, 1);
  if (sc_memoryLoad (buff) != 0)
    {
      mt_setbgcolor (RED);
      write (STDOUT_FILENO, " Fail! \n", strlen (" Fail! \n"));
    }
  else
    {
      mt_setbgcolor (GREEN);
      write (STDOUT_FILENO, " Success! \n", strlen (" Success! \n"));
    }
  mt_setbgcolor (GREY);
  read (STDOUT_FILENO, NULL, 1);
  g_drawboxes ();
  return 0;
}

int
g_savememory (void)
{
  mt_gotoXY (25, 1);
  write (STDOUT_FILENO, "Save to:>", strlen ("Save to:>"));
  char buff[20][1];
  mt_gotoXY (25, 11);
  for (int i = 0; i < 20; i++)
    {
      mt_gotoXY (25, 12 + i);
      read (STDOUT_FILENO, buff[i], 1);
      if (buff[i][0] == '\n')
        {
          break;
        }
      mt_gotoXY (25, 12);
      write (STDERR_FILENO, buff, strlen (buff));
    }
  buff[strlen (buff) - 1][0] = '\0';
  sc_memorySave (buff);
  mt_gotoXY (26, 1);
  mt_setbgcolor (GREEN);
  write (STDOUT_FILENO, " Success! \n", strlen (" Success! \n"));
  mt_setbgcolor (GREY);
  read (STDOUT_FILENO, NULL, 1);
  g_drawboxes ();
  return 0;
}

int
g_setmemory (int x, int y)
{
  mt_gotoXY (25, 1);
  write (STDOUT_FILENO, "Set memory value to:>",
         strlen ("Set memory value to:>"));
  char buff[5][1];
  for (int i = 0; i < 4; i++)
    {
      mt_gotoXY (25, 23 + i);
      read (STDOUT_FILENO, buff[i], 1);
      if (buff[i][0] == '\n')
        {
          break;
        }
      mt_gotoXY (25, 23);
      write (STDERR_FILENO, buff, strlen (buff));
    }
  buff[4][0] = '\0';
  int val = atoi (buff);
  sc_memorySet (x * 10 + y, val);
  g_drawboxes ();
  return 0;
}

int
g_getunit (int address, char *buff)
{
  int val, command, operand;
  sc_memoryGet (address, &val);
  char temp[6];
  int err = sc_commandDecode (val, &command, &operand);
  sprintf (temp, " %02x%02x", command, operand);
  if (err == ERR_WRONG_COMMAND)
    {
      temp[0] = '-';
    }
  else if (err == 0)
    {
      temp[0] = '+';
    }
  strcpy (buff, temp);
  return 0;
}

int
g_highlightmemory (int x, int y)
{
  g_drawmemorybox ();
  int address = x * 10 + y;
  char buff[6];
  g_getunit (address, &buff);
  mt_gotoXY (X_START + x, Y_START + y * Y_STEP);
  mt_setbgcolor (BLUE);
  write (STDERR_FILENO, buff, 6 * sizeof (char));
  mt_setbgcolor (GREY);
  return 0;
}

int
g_drawoperationbox (void)
{
  mt_gotoXY (8, 79);
  int address;
  sc_countGet (&address);
  char buff[6];
  g_getunit (address, &buff);
  char result[7];
  sprintf (result, "%c%c%c:%c%c", buff[0], buff[1], buff[2], buff[3], buff[4]);
  write (STDOUT_FILENO, result, 7 * sizeof (char));
  mt_gotoXY (33, 0);
  return 0;
}

int
g_drawcounterbox (void)
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
g_drawmemorybox (void)
{
  int k = 0;
  for (int i = X_START; i < 12; i++)
    {
      for (int j = Y_START; j < 63; j += Y_STEP)
        {
          mt_gotoXY (i, j);
          char buff[6];
          g_getunit (k++, &buff);
          write (STDERR_FILENO, buff, 6 * sizeof (char));
        }
    }
  return 0;
}

int
g_drawboxes ()
{
  g_drawmemorybox ();
  g_drawaccumbox ();
  g_drawcounterbox ();
  g_drawoperationbox ();
  g_drawflagbox ();
  g_drawbcbox ();
  return 0;
}

int
g_interface ()
{
  mt_setbgcolor (GREY);
  g_drawborders ();
  int exit = 0, x = 0, y = 0, frame = 0;
  while (!exit)
    {
      g_drawboxes ();
      int count, flag;
      sc_countGet (&count);
      x = count / 10;
      y = count % 10;
      g_highlightmemory (x, y);
      enum keys key = KEY_DEFAULT;
      rk_readkey (&key);
      switch (key)
        {
        case KEY_Q:
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
          raise (SIGUSR1);
          x = 0, y = 0;
          g_drawboxes ();
          break;
        case KEY_ENTER:
          g_setmemory (x, y);
          break;
        case KEY_R:
          sc_regGet (FLAG_IGNORE, &flag);
          sc_regSet (FLAG_IGNORE, !flag);
          alarm (flag);
          break;
        case KEY_T:

          break;
        default:
          break;
        }
      if (frame == 5)
        {
          g_drawborders ();
          frame = 0;
        }
      if (x == -1)
        {
          x = 9;
        }
      if (y == -1)
        {
          y = 9;
        }
      sc_countSet (x * 10 + y);
      frame++;
    }
  return 0;
}