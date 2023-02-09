#include "msc.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int sc_memory[MEMSIZE];
int sc_register;

int
sc_memoryInit () // initializes the array of 100 elements
{
  memset (sc_memory, 0, MEMSIZE * sizeof (sc_memory[0]));
  return 0;
}

int
sc_memorySet (int address,
              int value) // sets the value of [address] memory unit
{
  if (address < 0 || address >= MEMSIZE)
    {
      BIT_SET (sc_register, FLAG_WRONG_ADDRESS);
      return FLAG_WRONG_ADDRESS;
    }
  sc_memory[address] = value;
  return 0;
}

int
sc_memoryGet (int address, // gets the value of [address] memory unit and
              int *value)  // returns it into value var
{
  if (address < 0 || address >= MEMSIZE)
    {
      BIT_SET (sc_register, FLAG_WRONG_ADDRESS);
      return FLAG_WRONG_ADDRESS;
    }
  *value = sc_memory[address];
  return 0;
}

int
sc_memorySave (
    char *filename) // saves memory into a binary file (write/fwrite)
{
  FILE *f = fopen (filename, "wb");
  if (!f)
    {
      return 1;
    }
  fwrite (sc_memory, sizeof (int), sizeof (sc_memory) / sizeof (int), f);
  fclose (f);
  return 0;
}

int
sc_memoryLoad (char *filename) // loads RAM from a file (read/fread)
{
  FILE *f = fopen (filename, "rb");
  if (!f)
    {
      return 1;
    }
  fread (sc_memory, sizeof (int), sizeof (sc_memory) / sizeof (int), f);
  fclose (f);
  return 0;
}

int
sc_regInit (void) // inits the register of flags with 0
{
  sc_register = 0;
  return 0;
}

int
sc_regSet (int reg,   // sets the flag register value, #define-s are used for
           int value) // register numbers, if wrong register number - error
{
  if (reg < 0 || reg > 4)
    {
      return ERR_WRONG_FLAG;
    }
  if (!value)
    {
      BIT_DEL (sc_register, reg);
      return 0;
    }
  if (value != 1)
    {
      BIT_SET (sc_register, FLAG_OVERFLOW);
      return ERR_WRONG_VALUE;
    }
  BIT_SET (sc_register, reg);
  return 0;
}

int
sc_regGet (int reg,
           int *value) // gets the flag value, if wrong register - error
{
  if (reg < 0 || reg > 4)
    {
      return ERR_WRONG_FLAG;
    }
  *value = BIT_GET (sc_register, reg);
  return 0;
}

int
sc_commandEncode (int command, // encodes command with a specific number and
                  int operand, // operand, puts the result in value, if wrong
                  int *value)  // command or operand - error, value not changes
{
}

int
sc_commandDecode (
    int value,
    int *command, // decodes value as a sc command, if decoding is impossible -
    int *operand) // sets error command and returns an error
{
}