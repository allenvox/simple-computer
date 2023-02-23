#include "term_gui.h"
#include "term.h"
#include "bc.h"
#include "msc.h"
#include "readkey.h"

char *
g_flags ()
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
  return buff;
}

void
g_static ()
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
  char *str[7] = {"l - load", "s - save", "r - run", "t - step", "i - reset", "F5 - accumulator", "F6 - instructionCounter"};
  for (int i = 0; i < 7; i++)
    {
      mt_gotoXY (15 + i, 66);
      write (STDOUT_FILENO, str[i], strlen (str[i]));
    }
  mt_gotoXY (33, 0);
}

void
g_memorybox ()
{
  int k = 0;
  for (int i = 2; i < 12; i++)
    {
      for (int j = 3; j < 63; j += 6)
        {
          mt_gotoXY (i, j);
          char buff[6];
          int val;
          sc_memoryGet (k++, &val);
          sprintf (buff, "+%04d", val);
          write (STDERR_FILENO, buff, 6 * sizeof (char));
        }
    }
}

void
g_accumbox ()
{
  mt_gotoXY (2, 80);
  char buff[5];
  int val;
  sc_accumGet (&val);
  sprintf (buff, "%04x", val);
  write (STDOUT_FILENO, buff, 5 * sizeof (char));
  mt_gotoXY (33, 0);
}

void
g_counterbox ()
{
  mt_gotoXY (5, 80);
  char buff[5];
  int val;
  sc_countGet (&val);
  sprintf (buff, "%04x", val);
  write (STDOUT_FILENO, buff, 5 * sizeof (char));
  mt_gotoXY (33, 0);
}

void
g_operationbox ()
{
  mt_gotoXY (8, 79);
  write (STDOUT_FILENO, "+00 : 00", 8 * sizeof (char));
  mt_gotoXY (33, 0);
}

void
g_flagbox ()
{
  mt_gotoXY (11, 79);
  write (STDOUT_FILENO, g_flags(), 14 * sizeof (char));
  mt_gotoXY (33, 0);
}

void
g_bcbox (int *big)
{
  for (int i = 0; i < 6; i++)
    {
      int digit[2] = {big[2 * i], big[2 * i + 1]};
      bc_printbigchar (digit, BC_X, BC_START + i * BC_STEP, GREEN, GREY);
    }
  mt_gotoXY (33, 0);
}