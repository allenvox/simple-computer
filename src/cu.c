#include "cu.h"
#include "alu.h"
#include "term.h"
#include "tui.h"

int
READ (int operand) // read new content for memory unit from console
{
  g_clearfields ();
  mt_gotoXY (INPUTFIELD_X, 1);
  mt_printtext (" Input (hex):> ");
  rk_mytermsave ();
  rk_mytermregime (0, 0, 4, 1, 1);
  char buff[5];
  mt_readtext (buff, sizeof (buff));
  int value;
  sscanf (buff, "%x", &value);
  sc_regSet (FLAG_WRONG_COMMAND, 0);
  sc_regSet (FLAG_WRONG_OPERAND, 0);
  int cmd, oper;
  int err = sc_commandDecode (value, &cmd, &oper);
  mt_gotoXY (RESULTFIELD_X, 1);
  mt_printtext (" ");
  if (err == 0)
    {
      sc_memorySet (operand, value);
      mt_setbgcolor (GREEN);
      mt_printtext (" SUCCESS ");
    }
  else
    {
      sc_memorySet (operand, value);
      mt_setbgcolor (RED);
      mt_printtext (" FAIL : WRONG ");
      if (err == ERR_WRONG_COMMAND)
        {
          sc_regSet (FLAG_WRONG_COMMAND, 1);
          mt_printtext ("COMMAND");
        }
      else
        {
          sc_regSet (FLAG_WRONG_OPERAND, 1);
          mt_printtext ("OPERAND");
        }
      mt_printtext (" : WROTE ANYWAY ");
    }
  mt_setbgcolor (GREY);
  rk_mytermrestore ();
  g_drawmemorybox ();
  return 0;
}

int
WRITE (int operand) // write memory unit contents to console
{
  g_clearfields ();
  char buff[6];
  g_getunit (operand, &buff);
  char tmp[16];
  sprintf (tmp, " Output (hex):> %s", buff);
  mt_gotoXY (RESULTFIELD_X, 1);
  mt_printtext (tmp);
  return 0;
}

int
LOAD (int operand) // put value from accumulator to operand# memory cell
{
  int value = 0;
  sc_memoryGet (operand, &value);
  sc_accumSet (value);
  return 0;
}

int
STORE (int operand) // put operand from accumulator to memory
{
  int accum;
  sc_accumGet (&accum);
  sc_memorySet (operand, accum);
  return 0;
}

int
JUMP (int operand) // jump to instruction
{
  sc_countSet (operand);
  CU ();
  return 0;
}

int
JNEG (int operand) // jump to instruction if accumulator is negative
{
  int accum;
  sc_accumGet (&accum);
  if (accum < 0)
    {
      sc_countSet (operand);
      CU ();
    }
  return 0;
}

int
JZ (int operand) // jump if accumulator equals to zero
{
  int accum;
  sc_accumGet (&accum);
  if (accum == 0)
    {
      sc_countSet (operand);
      CU ();
    }
  return 0;
}

int
HALT () // set ignore flag to 1
{
  sc_regSet (FLAG_IGNORE, 1);
  alarm (0);
  sc_countSet (0);
  return 0;
}

int
JNS (int operand)
{
  int accum;
  sc_accumGet (&accum);
  if (accum > 0)
    {
      sc_countSet (operand);
    }
  return 0;
}

int
CU ()
{
  int value = 0, count;
  sc_countGet (&count);
  sc_memoryGet (count, &value);
  int command, operand;
  if (sc_commandDecode (value, &command, &operand) < 0)
    {
      sc_regSet (FLAG_IGNORE, 1);
      return 0;
    }
  if (((command >= 0x30) && (command <= 0x33)) || (command == 0x52))
    {
      ALU (command, operand);
    }
  else
    {
      switch (command)
        {
        case 0x10:
          READ (operand);
          break;
        case 0x11:
          WRITE (operand);
          break;
        case 0x20:
          LOAD (operand);
          break;
        case 0x21:
          STORE (operand);
          break;
        case 0x40:
          JUMP (operand);
          break;
        case 0x41:
          JNEG (operand);
          break;
        case 0x42:
          JZ (operand);
          break;
        case 0x43:
          HALT ();
          break;
        case 0x55:
          JNS (operand);
          break;
        }
    }
  return 0;
}