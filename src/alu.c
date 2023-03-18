#include "alu.h"

int
ALU (int command, int operand)
{
  int tmp, accum;
  sc_memoryGet (operand, &tmp);
  sc_accumGet (&accum);
  switch (command)
	{
	  case 0x30: // summation
		sc_accumSet (accum + tmp);
		break;
	  case 0x31: // substraction
		sc_accumSet (accum - tmp);
		break;
	  case 0x32: // division
		if (tmp == 0)
		  {
			sc_regSet (FLAG_DIV_BY_ZERO, 1);
			return -1;
		  }
		sc_regSet (FLAG_DIV_BY_ZERO, 0);
		sc_accumSet (accum / tmp);
		break;
	  case 0x33: // multiplication
        sc_accumSet (accum * tmp);
		break;
	  case 0x52: // conjunction
		int a = accum;
		int b = tmp;
		for (int i = 0; i < 8; i++)
		  {
			if ((a & (1 << i)) & (b & (1 << i)))
			  {
                sc_accumSet (accum | (1 << i));
			  }
		  }
	    break; 
	}
    return 0;
}