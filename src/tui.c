#include "tui.h"
#include "cu.h"

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
  sprintf (buff, "%d", val);
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
g_clearfields ()
{
  for (int j = 25; j < 28; j++)
    {
      mt_gotoXY (j, 1);
      for (int i = 0; i < 102; i++)
        {
          mt_printtext (" ");
        }
    }
  return 0;
}

int
g_loadmemory (void)
{
  g_clearfields ();
  mt_gotoXY (INPUTFIELD_X, 1);
  mt_printtext (" Load from file:> ");
  rk_mytermregime (0, 0, 1, 1, 1);
  char *buff = calloc (0, sizeof (char) * 100);
  scanf ("%s", buff);
  mt_gotoXY (RESULTFIELD_X, 1);
  mt_printtext (" ");
  if (sc_memoryLoad (buff) == 0)
    {
      mt_setbgcolor (GREEN);
      mt_printtext (" SUCCESS ");
    }
  else
    {
      mt_setbgcolor (RED);
      mt_printtext (" FAIL ");
    }
  mt_setbgcolor (GREY);
  g_drawboxes ();
  return 0;
}

int
g_savememory (void)
{
  g_clearfields ();
  mt_gotoXY (INPUTFIELD_X, 1);
  mt_printtext (" Save to file:> ");
  rk_mytermregime (0, 0, 1, 1, 1);
  char *buff = calloc (0, sizeof (char) * 100);
  scanf ("%s", buff);
  sc_memorySave (buff);
  mt_gotoXY (RESULTFIELD_X, 1);
  mt_printtext (" ");
  mt_setbgcolor (GREEN);
  mt_printtext (" SUCCESS ");
  mt_setbgcolor (GREY);
  g_drawboxes ();
  return 0;
}

int
g_getunit (int address, char *buff)
{
  int val, command, operand;
  sc_memoryGet (address, &val);
  if (val < 0)
    {
      sprintf (buff, "-%04x", abs (val));
      return 0;
    }
  char temp[6];
  int err = sc_commandDecode (val, &command, &operand);
  sprintf (temp, " %04x", val);
  if (err == 0)
    {
      temp[0] = '+';
    }
  else
    {
      temp[0] = '-';
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
  mt_printtext (buff);
  mt_setbgcolor (GREY);
  return 0;
}

int
g_drawoperationbox (void)
{
  mt_gotoXY (8, 79);
  int address, val, cmd = 0, oper = 0;
  sc_countGet (&address);
  sc_memoryGet (address, &val);
  char buff[11];
  int err = sc_commandDecode (val, &cmd, &oper);
  sprintf (buff, " %02x:%d", cmd, oper);
  if (err == 0)
    {
      buff[0] = '+';
    }
  else
    {
      buff[0] = '-';
    }
  mt_printtext (buff);
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
  g_clearfields ();
  int exit = 0, address = 0, flag = 0;
  while (!exit)
    {
      sc_countGet (&address);
      g_drawboxes ();
      int x = address / 10, y = address % 10;
      g_highlightmemory (x, y);
      enum keys key = KEY_DEFAULT;
      rk_readkey (&key);
      switch (key)
        {
        case KEY_Q: // exit
          exit++;
          break;
        case KEY_UP:
          address -= 10;
          if (address < 0)
            {
              address += 100;
            }
          sc_countSet (address);
          break;
        case KEY_DOWN:
          address += 10;
          if (address > 99)
            {
              address -= 100;
            }
          sc_countSet (address);
          break;
        case KEY_LEFT:
          address -= 1;
          if (address % 10 == 9 || address == -1)
            {
              address += 10;
            }
          sc_countSet (address);
          break;
        case KEY_RIGHT:
          address += 1;
          if (address % 10 == 0)
            {
              address -= 10;
            }
          sc_countSet (address);
          break;
        case KEY_L: // load memory
          g_loadmemory ();
          break;
        case KEY_S: // save memory
          g_savememory ();
          break;
        case KEY_I: // reset
          raise (SIGUSR1);
          x = 0, y = 0;
          g_drawboxes ();
          break;
        case KEY_ENTER: // setting values
          READ (address);
          break;
        case KEY_R: // ignore flag
          sc_regGet (FLAG_IGNORE, &flag);
          sc_regSet (FLAG_IGNORE, !flag);
          alarm (flag);
          break;
        case KEY_T: // step key - calling control unit
          CU ();
          break;
        case KEY_F5:
          break;
        case KEY_F6:
          break;
        }
    }
  return 0;
}