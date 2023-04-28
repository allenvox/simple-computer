#include "msc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input = NULL;

void
errOutput (char *message)
{
  fprintf (stderr, "%s", message);
  exit (EXIT_FAILURE);
}

void
loadFile (const char *filename)
{
  if ((input = fopen (filename, "r")) == NULL)
    {
      errOutput ("Input file does not exist\n");
    }
}

int
getCommandNum (char *command)
{
  int result = 0;
  if (strcmp (command, "READ") == 0)
    {
      result = 0x10;
    }
  else if (strcmp (command, "WRITE") == 0)
    {
      result = 0x11;
    }
  else if (strcmp (command, "LOAD") == 0)
    {
      result = 0x20;
    }
  else if (strcmp (command, "STORE") == 0)
    {
      result = 0x21;
    }
  else if (strcmp (command, "ADD") == 0)
    {
      result = 0x30;
    }
  else if (strcmp (command, "SUB") == 0)
    {
      result = 0x31;
    }
  else if (strcmp (command, "DIVIDE") == 0)
    {
      result = 0x32;
    }
  else if (strcmp (command, "MUL") == 0)
    {
      result = 0x33;
    }
  else if (strcmp (command, "JUMP") == 0)
    {
      result = 0x40;
    }
  else if (strcmp (command, "JNEG") == 0)
    {
      result = 0x41;
    }
  else if (strcmp (command, "JZ") == 0)
    {
      result = 0x42;
    }
  else if (strcmp (command, "HALT") == 0)
    {
      result = 0x43;
    }
  else if (strcmp (command, "AND") == 0)
    {
      result = 0x52;
    }
  else if (strcmp (command, "JNS") == 0)
    {
      result = 0x55;
    }
  else
    {
      char errMsg[64];
      sprintf (errMsg, "Not recognized command %s\n", command);
      errOutput (errMsg);
    }
  return result;
}

void
writeTranslationTo (const char *filename)
{
  for (int i = 0; !feof (input); i++)
    {
      char line[255];
      if (!fgets (line, 255, input))
        {
          if (feof (input))
            {
              break;
            }
          else
            {
              char errMsg[64];
              sprintf (errMsg, "Can't read line %d from input file\n", i);
              errOutput (errMsg);
            }
        }

      char *ptr = strtok (line, " ");
      char *str_address = ptr;
      int address = atoi (str_address);
      if (address < 0 || address > 99)
        {
          char errMsg[64];
          sprintf (errMsg, "Expected address of memory cell on line %d\n", i);
          errOutput (errMsg);
        }

      ptr = strtok (NULL, " ");
      char *command = ptr;

      ptr = strtok (NULL, " +");
      char *str_operand = ptr;

      ptr = strtok (NULL, " ");

      if (ptr != NULL && ptr[0] != ';')
        {
          char errMsg[64];
          sprintf (errMsg, "Unexpected symbols on line %d\n", i);
          errOutput (errMsg);
        }

      int operand = atoi (str_operand);
      char buffer[255];
      sprintf (buffer, "%02x", operand);
      sscanf (buffer, "%02x", &operand);

      if (command[0] == '=')
        {
          sscanf (str_operand, "%x", &operand);
          sc_memorySet (address, operand);
          continue;
        }
      int num_of_cmd = getCommandNum (command);

      // command encoding, correctness checking, writing to mem
      int value = 0;
      if (sc_commandEncode (num_of_cmd, operand, &value) < 0)
        {
          char errMsg[64];
          sprintf (errMsg, "Encoding error on line %d\n", i);
          errOutput (errMsg);
        }
      sc_memorySet (address, value);
    }

  // write the result memory to output file
  sc_memorySave (strdup (filename));
}

int
main (int argc, const char **argv)
{
  if (argc < 3)
    {
      errOutput ("Usage: ./sat <input_file.sa> <output_object_file.o>\n");
    }
  sc_memoryInit ();
  loadFile (argv[1]);
  writeTranslationTo (argv[2]);
  return 0;
}