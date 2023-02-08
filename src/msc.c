#include "msc.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

int sc_memory[MEMSIZE];

int
sc_memoryInit () // initializes the array of 100 elements
{
    memset(sc_memory, 0, MEMSIZE * sizeof(sc_memory[0]));
    return 0;
}

int
sc_memorySet (int address,
              int value) // sets the value of [address] memory unit
{
}

int
sc_memoryGet (int address,
              int *value) // gets the value of [address] memory unit and
                          // returns it into value var
{
}

int
sc_memorySave (
    char *filename) // saves memory into a binary file (write/fwrite)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        return 1;
    }
    fwrite(sc_memory, sizeof(int), sizeof(sc_memory) / sizeof(int), f);
    fclose(f);
    return 0;
}

int
sc_memoryLoad (char *filename) // loads RAM from a file (read/fread)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        return 1;
    }
    fread(sc_memory, sizeof(int), sizeof(sc_memory) / sizeof(int), f);
    fclose(f);
    return 0;
}

int
sc_regInit (void)
{ // inits the register of flags with 0
}

int
sc_regSet (int reg,
           int value) // sets the flag register value, #define-s are used for
                      // register numbers, if wrong register number - error
{
}

int
sc_regGet (int reg,
           int *value) // gets the flag value, if wrong register - error
{
}

int
sc_commandEncode (int command, int operand,
                  int *value) // encodes command with a specific number and
                              // operand, puts the result in value, if wrong
                              // command or operand - error, value not changes
{
}

int
sc_commandDecode (
    int value, int *command,
    int *operand) // decodes value as a sc command, if decoding is impossible -
                  // sets error command and returns an error
{
}