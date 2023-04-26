#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input = NULL, *output = NULL;

void
loadFiles (const char *input_file, const char *output_file)
{
  if ((input = fopen (input_file, "r")) == NULL)
    {
      fprintf (stderr, "Input file '%s' does not exist\n", input_file);
      exit (EXIT_FAILURE);
    }
  output = fopen (output_file, "w");
}

int
isVariable (char name)
{
  return (name >= 'A' || name <= 'Z');
}

void
INPUT (char var)
{
  if (!isVariable(var))
    {
      fprintf (stderr, "Wrong variable name\n");
      exit (EXIT_FAILURE);
    }
  fprintf (output, "%.2i READ %d\n", assemblerCommandCounter,
           getVariableAddress (var));
  assemblerCommandCounter++;
}

void
PRINT (char var)
{
  if (!isVariable(var))
    {
      fprintf (stderr, "Wrong variable name.\n");
      exit (EXIT_FAILURE);
    }
  fprintf (output, "%.2i WRITE %d\n", assemblerCommandCounter,
           getVariableAddress (var));
  assemblerCommandCounter++;
}

void
END ()
{
  fprintf (output, "%.2i HALT 00\n", assemblerCommandCounter);
  assemblerCommandCounter++;
}

/*void INPUT (char *arguments);
void PRINT (char *arguments);
void IF (char *arguments);
void GOTO (int address, int operand);
void function (char *command, char *arguments);
void parseRPN (char *rpn, char var);
void LET (char *arguments);
void END ();*/

typedef struct variable
{
  char name;
  int address;
  int value;
} variable;

variable variables[52];
int variableCount = 0;
char lastConstantName = 'a';

typedef struct command
{
  int num;
  char *command;
  int address;
} command;

int basicCommandCouner = 0, assemblerCommandCounter = 0, gotoCounter = -1;
command *program, *gotoRecords;

int
getVariableAddress (char name)
{
  for (int i = 0; i < 52; i++)
    {
      if (variables[i].name == name)
        {
          return variables[i].address;
        }
      if (variables[i].name == NULL)
        {
          variables[i].name = name;
          variables[i].address = 99 - i;
          variableCount++;
          return variables[i].address;
        }
    }
  return 0;
}

char
intToConstant (int value)
{
  for (int i = 0; i < 52; i++)
    {
      if (variables[i].name == NULL)
        {
          variables[i].name = lastConstantName;
          lastConstantName++;
          variables[i].address = 99 - i;
          variables[i].value = value;
          fprintf (output, "%.2i = %04x\n", assemblerCommandCounter,
                   abs (variables[i].value));
          variableCount++;
          assemblerCommandCounter++;
          return variables[i].name;
        }
      if (variables[i].value != NULL)
        {
          if (variables[i].value == value)
            {
              return variables[i].name;
            }
        }
    }
}

char
preCalcProcessing (char *expr)
{
  char *ptr = strtok (expr, " =");
  char val;
  sscanf (ptr, "%s", &val);
  if (!isVariable(val))
    {
      fprintf (stderr, "Incorrect variable\n");
      exit (EXIT_FAILURE);
    }
  ptr = strtok (NULL, " ");
  char *equal = ptr;
  if (strcmp (equal, "=") != 0)
    {
      fprintf (stderr, "Wrong expression\n");
      exit (EXIT_FAILURE);
    }
  ptr = strtok (NULL, "");
  char *exp = ptr;
  int i = 0, j = 0, pos = 0, operat = 0, flg = 1, m = 0;
  char *assign = (char *)malloc (sizeof (char) * 255);
  for (int k = 0; k < (int)strlen (exp); k++)
    {
      if (exp[k] == '-' && flg)
        {
          assign[m] = '0';
          m++;
        }
      flg = 0;
      if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*')
        {
          operat++;
        }
      if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*'
          || exp[k] == '(')
        {
          flg = 1;
        }
      assign[m] = exp[k];
      m++;
    }
  if (operat == 0) // 0+ перед ним, если перед минусом нет аргумента, то пишем
                   // 0 перед миунсом
    {
      sprintf (exp, "0 + %s", assign);
    }
  else
    {
      sprintf (exp, "%s", assign);
    }
  while (exp[i] != '\n' && exp[i] != '\0')
    {
      if (exp[i] >= '0' && exp[i] <= '9')
        {
          char num[256];
          j = 0;
          num[j] = exp[i];
          j++;
          pos = i;
          exp[i] = ' ';
          i++;
          while (exp[i] >= '0' && exp[i] <= '9')
            {
              num[j] = exp[i];
              j++;
              exp[i] = ' ';
              i++;
            }
          num[j] = '\0';
          exp[pos] = intToConstant (atoi (num));
        }
      i++;
    }
  sprintf (expr, "%s", exp);
  return val;
}

void
basic_translate ()
{
  int instructionCounter = 1;
  while (1)
    {
      char temp[255];
      fgets (temp, 254, input);
      if (feof (input))
        {
          break;
        }
      instructionCounter++;
    }
  basicCommandCouner = instructionCounter;
  rewind (input);
  program = (command *)malloc (sizeof (command) * instructionCounter);
  gotoRecords = (command *)malloc (sizeof (command) * instructionCounter);
  for (int i = 0; i < instructionCounter; i++)
    {
      program[i].command = (char *)malloc (sizeof (char) * 255);
      if (!fgets (program[i].command, 254, input))
        {
          if (feof (input))
            {
              return;
            }
          else
            {
              fprintf (stderr, "Line %d: can't read line from file\n", i++);
              exit (EXIT_FAILURE);
            }
        }
    }
  for (int i = 0; i < instructionCounter; i++)
    {
      char *lin;
      char *thisCommand = (char *)malloc (sizeof (char) * 255);
      sprintf (thisCommand, "%s", program[i].command);
      char *ptr = strtok (thisCommand, " ");
      lin = ptr;
      int line = atoi (lin);
      if ((strcmp (lin, "0") == 0) || (strcmp (lin, "00") == 0))
        {
          fprintf (stderr, "Line %d: expected line number\n", i++);
          exit (EXIT_FAILURE);
        }
      if (i - 1 >= 0)
        {
          if (line <= program[i - 1].num)
            {
              fprintf (stderr, "Line %d: wrong line number\n", i++);
              exit (EXIT_FAILURE);
            }
        }
      program[i].num = line;
      char *command;
      ptr = strtok (NULL, " ");
      command = ptr;
      char *arguments;
      ptr = strtok (NULL, "");
      arguments = ptr;
      program[i].address = assemblerCommandCounter;
      if (strcmp (command, "GOTO") != 0)
        {
          function (command, arguments);
        }
      else
        {
          gotoCounter++;
          gotoRecords[gotoCounter].num = program[i].num;
          gotoRecords[gotoCounter].command = program[i].command;
          gotoRecords[gotoCounter].address = program[i].address;
          assemblerCommandCounter++;
        }
    }
  for (int i = 0; i <= gotoCounter; i++)
    {
      int address = gotoRecords[i].address;
      char *ptr = strtok (gotoRecords[i].command, " ");
      ptr = strtok (NULL, " ");
      ptr = strtok (NULL, "");
      int operand = atoi (ptr);
      GOTO (address, operand);
    }
}

void
parseRPN (char *rpn, char var)
{
  int i = 0, depth = 0, operand1, operand2;
  char memoryCounter = '1';
  while (rpn[i] != '\0' && rpn[i] != '\n')
    {
      char x = rpn[i];
      if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
        {
          fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
                   getVariableAddress (x));
          assemblerCommandCounter++;
          fprintf (output, "%.2i STORE %d\n", assemblerCommandCounter,
                   getVariableAddress (memoryCounter));
          memoryCounter++;
          assemblerCommandCounter++;
          depth++;
        }
      if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          if (depth < 2)
            {
              fprintf (stderr, "Incorrect LET statement\n");
              exit (EXIT_FAILURE);
            }
          else
            {
              operand1 = getVariableAddress (memoryCounter - 2);
              operand2 = getVariableAddress (memoryCounter - 1);
              fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
                       operand1); // right operand goes to accum
              assemblerCommandCounter++;
              switch (x)
                {
                case '+': // summation
                  fprintf (output, "%.2i ADD %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                case '-': // substraction
                  fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                case '/': // division
                  fprintf (output, "%.2i DIVIDE %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                case '*': // multiplication
                  fprintf (output, "%.2i MUL %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                }
              fprintf (output, "%.2i STORE %d\n", assemblerCommandCounter,
                       getVariableAddress (memoryCounter - 2));
              assemblerCommandCounter++;
              depth--;
              memoryCounter = memoryCounter - 1;
            }
        }
      i++;
    }
  if (depth != 1)
    {
      fprintf (stderr, "Incorrect LET statement\n");
      exit (EXIT_FAILURE);
    }
  fprintf (output, "%.2i STORE %d\n", assemblerCommandCounter,
           getVariableAddress (var));
  assemblerCommandCounter++;
}

void
GOTO (int address, int operand)
{
  for (int i = 0; i < basicCommandCouner; i++)
    {
      if (program[i].num == operand)
        {
          fprintf (output, "%.2i JUMP %d\n", address, program[i].address);
        }
    }
  fprintf (stderr, "Reference to an inspecifed memory location\n");
}

void
IF (char *arguments)
{
  int mySign = -1, before = 0, after = 0;
  for (int i = 0; i < (int)strlen (arguments); i++)
    {
      if ((arguments[i] == '>') || (arguments[i] == '<')
          || (arguments[i] == '='))
        {
          mySign = i;
          if (!(arguments[i - 1] == ' '))
            {
              before = 1;
            }
          if (!(arguments[i + 1] == ' '))
            {
              after = 1;
            }
          break;
        }
    }
  char *expression = (char *)malloc (sizeof (char) * 255);
  if (!(before) && !(after))
    {
      expression = strtok (arguments, "");
    }
  else
    {
      int j = 0;
      for (int i = 0; i < (int)strlen (arguments); i++)
        {
          if (i == mySign)
            {
              if (before)
                {
                  expression[j] = ' ';
                  j++;
                }
              expression[j] = arguments[i];
              j++;
              if (after)
                {
                  expression[j] = ' ';
                  j++;
                }
            }
          else
            {
              expression[j] = arguments[i];
              j++;
            }
        }
      expression[j] = '\0';
    }
  char *ptr = strtok (expression, " ");
  char *operand1 = ptr;
  char operand1Name;
  if (strlen (operand1) > 1)
    {
      if (atoi (operand1))
        {
          operand1Name = intToConstant (atoi (operand1));
        }
    }
  else
    {
      if ((operand1[0] >= '0') && (operand1[0] <= '9'))
        {
          operand1Name = intToConstant (atoi (operand1));
        }
      else if ((operand1[0] >= 'A') && (operand1[0] <= 'Z'))
        {
          operand1Name = operand1[0];
        }
      else
        {
          fprintf (stderr, "Wrong statement\n");
          exit (EXIT_FAILURE);
        }
    }
  ptr = strtok (NULL, " ");
  char *logicalSign = ptr;
  ptr = strtok (NULL, " ");
  char *operand2 = ptr;
  char operand2Name;
  if (strlen (operand2) > 1)
    {
      if (atoi (operand2))
        {
          operand2Name = intToConstant (atoi (operand2));
        }
    }
  else
    {
      if ((operand2[0] >= '0') && (operand2[0] <= '9'))
        {
          operand2Name = intToConstant (atoi (operand2));
        }
      else if ((operand2[0] >= 'A') && (operand2[0] <= 'Z'))
        {
          operand2Name = operand2[0];
        }
      else
        {
          fprintf (stderr, "Wrong statement\n");
          exit (EXIT_FAILURE);
        }
    }
  int falsePosition = -1;
  if (logicalSign[0] == '<')
    {
      fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
               getVariableAddress (operand1Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
               getVariableAddress (operand2Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i JNEG %d\n", assemblerCommandCounter,
               assemblerCommandCounter + 2);
      assemblerCommandCounter++;
      falsePosition = assemblerCommandCounter;
      assemblerCommandCounter++;
    }
  else if (logicalSign[0] == '>')
    {
      fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
               getVariableAddress (operand2Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
               getVariableAddress (operand1Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i JNEG %d\n", assemblerCommandCounter,
               assemblerCommandCounter + 2);
      assemblerCommandCounter++;
      falsePosition = assemblerCommandCounter;
      assemblerCommandCounter++;
    }
  else if (logicalSign[0] == '=')
    {
      fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
               getVariableAddress (operand1Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
               getVariableAddress (operand2Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i JZ %d\n", assemblerCommandCounter,
               assemblerCommandCounter + 2);
      assemblerCommandCounter++;
      falsePosition = assemblerCommandCounter;
      assemblerCommandCounter++;
    }
  ptr = strtok (NULL, " ");
  char *command = ptr;
  ptr = strtok (NULL, "");
  char *commandArguments = ptr;
  if (strcmp (command, "IF") == 0)
    {
      fprintf (stderr, "Multiple IFs\n");
      exit (EXIT_FAILURE);
    }
  else if (strcmp (command, "GOTO") != 0)
    {
      function (command, commandArguments);
    }
  else
    {
      gotoCounter++;
      gotoRecords
          = realloc (gotoRecords, sizeof (command) * gotoCounter + 1);
      command gotoCommand;
      gotoCommand.address = assemblerCommandCounter;
      char *buff = (char *)malloc (sizeof (char) * 255);
      sprintf (buff, "%d GOTO %s", falsePosition, commandArguments);
      gotoCommand.command = buff;
      gotoRecords[gotoCounter] = gotoCommand;
      assemblerCommandCounter++;
    }
  fprintf (output, "%.2i JUMP %d\n", falsePosition, assemblerCommandCounter);
}

void
LET (char *arguments)
{
  char fin[255];
  char var = preCalcProcessing (arguments);
  translateToRPN (arguments, fin);
  parseRPN (fin, var);
}

void
function (char *command, char *arguments)
{
  if (strcmp (command, "REM") == 0)
    {
    }
  else if (strcmp (command, "INPUT") == 0)
    {
      INPUT (arguments);
    }
  else if (strcmp (command, "PRINT") == 0)
    {
      PRINT (arguments);
    }
  else if (strcmp (command, "IF") == 0)
    {
      IF (arguments);
    }
  else if (strcmp (command, "LET") == 0)
    {
      LET (arguments);
    }
  else if (strcmp (command, "END") == 0)
    {
      END ();
    }
  else
    {
      fprintf (stderr, "%s is not a command.\n", command);
    }
}

int
main (int argc, const char **argv)
{
  if (argc < 3)
    {
      fprintf (stderr,
               "Usage: %s <input_basic_file.sb> <output_assembler_file.sa>\n",
               argv[0]);
      exit (EXIT_FAILURE);
    }
  loadFiles (argv[1], argv[2]);
  basic_translate ();
  fclose (input);
  fclose (output);
  return 0;
}