#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "msc.h"

#define ERR_EXPECTED_ADDRESS_OF_MEMORY_CELL -1
#define ERR_WRONG_COMMAND -2
#define ERR_WRONG_OPERAND -3
#define ERR_BY_ENCODE -4

FILE *input = NULL;

void
load_file (const char* filename)
{
  if ((input = fopen (filename, "r")) == NULL)
	{
	  fprintf (stderr, "Can't open file: no such file.\n");
	  exit (EXIT_FAILURE);
	}
  return;
}

void
translation (const char* filename)
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
				fprintf (stderr, "Line %d: can't read line from file.\n", i++);
				break;
			}
		}
	  char* addr;
	  char* command;
	  char* oper;
	  int num_of_cmd = 0;
	  int operand = 0;
	  int address = 0;
	  char *ptr = strtok (line, " ");
	  addr = ptr;
	  ptr = strtok (NULL, " ");
	  command = ptr;
	  ptr = strtok (NULL, " +");
	  oper = ptr;
	  address = atoi (addr);
	  ptr = strtok (NULL," ");
	  if (ptr != NULL && ptr[0] != ';')
		{
		  fprintf (stderr, "Line %d: unexpected symbols.\n", ++i);
		  break;
		}
	  if (!atoi (addr) && strcmp (addr, "00") != 0)
		{
		  fprintf (stderr, "Line %d: expected address of memory cell.\n", ++i);
		  break;
		}
	  operand = atoi (oper);
	  char buffer[255];
	  sprintf (buffer, "%x", operand);
	  sscanf (buffer, "%x", &operand);
	  if((operand == NULL && (strcmp (oper, "0\n") != 0 && strcmp (oper, "00\n") != 0 && strcmp (oper, "000\n") != 0 && strcmp (oper, "0000\n") != 0)))
		{
		  if ((strcmp (command,"=") != 0))
			{
			  fprintf (stderr, "Line %d: wrong operand.\n", ++i);
			  exit (EXIT_FAILURE);
			}
		}
	  if (!strcmp (command, "READ"))
		{
		  num_of_cmd = 0x10;
		}
	  else if (!strcmp (command, "WRITE"))
		{
		  num_of_cmd = 0x11;
		}
	  else if (!strcmp (command, "LOAD"))
		{
		  num_of_cmd = 0x20;
		}
	  else if (!strcmp (command, "STORE"))
		{
		  num_of_cmd = 0x21;
		}
	  else if (!strcmp (command, "ADD"))
		{
		  num_of_cmd = 0x30;
		}
	  else if (!strcmp (command, "SUB"))
		{
		  num_of_cmd = 0x31;
		}
	  else if (!strcmp (command, "DIVIDE"))
		{
		  num_of_cmd = 0x32;
		}
	  else if (!strcmp (command, "MUL"))
		{
		  num_of_cmd = 0x33;
		}
	  else if (!strcmp (command, "JUMP"))
		{
		  num_of_cmd = 0x40;
		}
      else if (!strcmp (command, "JNEG"))
		{
		  num_of_cmd = 0x41;
		}
	  else if (!strcmp (command, "JZ"))
		{
		  num_of_cmd = 0x42;
		}
	  else if (!strcmp (command, "HALT"))
		{
		  num_of_cmd = 0x43;
		}
	  else if (!strcmp (command, "AND"))
		{
		  num_of_cmd = 0x52;
		}
	  else if (!strcmp (command, "JNS"))
		{
		  num_of_cmd = 0x55;
		}
	  else if (command[0] == '=')
		{
		  sscanf (oper, "%x", &operand);
		  sc_memorySet (address, operand);
		  continue;
		}
	  else
		{
		  fprintf (stderr, "Line %d: wrong command.\n", ++i );
		  break;
		}
		int value = 0;
	  if (sc_commandEncode (num_of_cmd, operand, &value) < 0)
		{
		  fprintf (stderr, "Line %d: error by encode. Check command or operand.\n", ++i );
		  break;
		}
	  sc_memorySet (address, value);
	}
  sc_memorySave (strdup (filename));
}

int
main (int argc, const char** argv)
{
  if (argc < 3)
	{
	  fprintf (stderr, "Usage: %s input.sa output.o\n", argv[0]);
	  exit (EXIT_FAILURE);
	}
  sc_memoryInit ();
  load_file (argv[1]);
  translation (argv[2]);
  return 0;
}