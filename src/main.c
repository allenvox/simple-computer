#include "msc.h"
#include <stdio.h>

int
main ()
{
  sc_memoryInit ();
  printf ("(init) memory[...] = 0\n");
  for (int i = 4; i < 10; i++)
    {
      int val = i*5;
      sc_memorySet (i, val);
      printf ("(set) sc_memory[%d] to %d\n", i, val);
    }
  for (int i = 4; i < 10; i++)
    {
      int val = 0;
      sc_memoryGet (i, &val);
      printf ("(get) sc_memory[%d] = %d\n", i, val);
    }
  char *memfile = "example.mem";
  sc_memorySave (memfile);
  printf ("(save) memory -> %s\n", memfile);
  for (int i = 4; i < 10; i++)
    {
      int val = i*3;
      sc_memorySet (i, val);
      printf ("(set) sc_memory[%d] to %d\n", i, val);
    }
  for (int i = 4; i < 10; i++)
    {
      int val = 0;
      sc_memoryGet (i, &val);
      printf ("(get) sc_memory[%d] = %d\n", i, val);
    }
  sc_memoryLoad (memfile);
  printf ("(load) memory <- %s\n", memfile);
  for (int i = 4; i < 10; i++)
    {
      int val = 0;
      sc_memoryGet (i, &val);
      printf ("(get) sc_memory[%d] = %d\n", i, val);
    }
  
  sc_regInit ();
  printf ("(init) register = 0\n");
  // ... ... 5 / 0 <-- OMG DIVISION BY ZERO
  sc_regSet (FLAG_DIV_BY_ZERO, 1);
  int val = 0;
  sc_regGet (FLAG_DIV_BY_ZERO, &val);
  printf ("(get) FLAG_DIV_BY_ZERO = %d\n", val);
  sc_regGet (FLAG_OVERFLOW, &val);
  printf ("(get) FLAG_OVERFLOW = %d\n", val);
  sc_regSet (FLAG_DIV_BY_ZERO, 2); // try to overflow the bit
  sc_regGet (FLAG_OVERFLOW, &val);
  printf ("(get) FLAG_OVERFLOW = %d\n", val);
  int command = 0x11; // "write" command code
  int operand = 14; // 14 is the value to "write"
  sc_commandEncode (command, operand, &val);
  printf ("(encode) command '%d' operand '%d' = '%d' SimpleComputer\n", command, operand, val);
  sc_commandDecode (val, &command, &operand);
  printf ("(decode) SimpleComputer '%d' = command '%d' operand '%d'\n", val, command, operand);
  sc_commandEncode (0x0, 129, &val); // try to encode wrong command
  printf ("(encode) value = %d\n", val); // not changes
  sc_regGet (FLAG_WRONG_COMMAND, &val);
  printf ("(get) FLAG_WRONG_COMMAND = %d\n", val);
  return 0;
}