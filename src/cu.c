#include "CU.h"

int
READ (int operand)
{
  mt_goToXY (25, 1);
  mt_printText ("Input value(dec):>");
  rk_mytermregime (0, 0, 1, 1, 1);
  char buff1[2];
  mt_readText (buff1, sizeof(buff1));
  int sign = 1;
  if (buff1[0] == '-' || buff1[0] == '+')
    {
      rk_mytermregime (0, 0, 4, 1, 1);
      if (buff1[0] == '-')
       	{
          sign = -1;
        }
    }
  else
    {
      rk_mytermregime (0, 0, 3, 1, 1);
    }
  char buff2 [5];
  mt_readText (buff2, sizeof(buff2));
  char buff3[6];
  if (sign)
    {
      sprintf (buff3, "%s", buff1);
    }
  sprintf (buff3, "%s%s", buff3, buff2);
  int value;
  sscanf (buff3, "%d", &value);
  sc_memorySet (operand, value);
  return 0;
}

int
WRITE (int operand)
{
  int value;
  sc_memoryGet (operand, &value);
  char tmp[14];
  if (value > 0)
	  {
	    sprintf (tmp, "Value:> %.4X", value);
	  }
  else
	  {
	    sprintf (tmp, "Value:> -%.4X", -1 * value);
	  }
  mt_printtext (tmp);
  return 0;
}

int
LOAD (int operand)
{
  int value = 0;
  sc_memoryGet (operand, &value);
  sc_accumSet (value);
  return 0;
}

int
STORE (int operand)
{
  int accum;
  sc_accumGet (&accum);
  sc_memorySet (operand, accum);
  return 0;
}

int
JUMP (int operand)
{
  sc_countSet (operand);
  CU ();
  return 0;
}

int
JNEG (int operand)
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
JZ (int operand)
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
HALT ()
{
  sc_regSet (FLAG_IGNORE, 1);
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