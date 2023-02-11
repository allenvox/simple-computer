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
  
  return 0;
}