#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int basic_counter = 0, assembler_counter = 0, goto_counter = 0, variable_counter = 0;
FILE *input = NULL, *output = NULL;

void
loadFiles (const char *input_file, const char *output_file)
{
  if ((input = fopen (input_file, "r")) == NULL)
    {
      errOutput ("Input file does not exist\n");
    }
  output = fopen (output_file, "w");
}

void
assemblerOutput (char *cmd, int operand)
{
  fprintf (output, "%.2i %s %d\n", assembler_counter, cmd, operand);
  assembler_counter++;
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
      errOutput("Wrong variable name\n");
    }
  assemblerOutput("READ", getVariableAddress(var));
}

void
PRINT (char var)
{
  if (!isVariable(var))
    {
      errOutput ("Wrong variable name.\n");
    }
  assemblerOutput ("WRITE", getVariableAddress(var));
}

void
GOTO (int address, int operand)
{
  for (int i = 0; i < basic_counter; i++)
    {
      if (all_commands_array[i].num == operand)
        {
          assemblerOutput ("JUMP", all_commands_array[i].address);
        }
    }
  errOutput ("Memory location specified in GOTO not found\n");
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
END ()
{
  assemblerOutput ("HALT", 0);
}

void
handle_basic_function (char *cmd, char *args)
{
  if (strcmp (cmd, "REM") == 0)
    {
    }
  else if (strcmp (cmd, "INPUT") == 0)
    {
      INPUT (args);
    }
  else if (strcmp (cmd, "PRINT") == 0)
    {
      PRINT (args);
    }
  else if (strcmp (cmd, "IF") == 0)
    {
      IF (args);
    }
  else if (strcmp (cmd, "LET") == 0)
    {
      LET (args);
    }
  else if (strcmp (cmd, "END") == 0)
    {
      END ();
    }
  else
    {
      char errMsg[64];
      sprintf (errMsg, "'%s' in not a valid command\n", cmd);
      errOutput (errMsg);
    }
}

/*
void IF (char *arguments);
void GOTO (int address, int operand);
void parseRPN (char *rpn, char var);
void LET (char *arguments);
*/

typedef struct variable
{
  char name;
  int address;
  int value;
} variable;

variable variables[52];
char lastConstantName = 'a';

typedef struct command
{
  int num;
  char *command;
  int address;
} command;

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
          variable_counter++;
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
          assemblerOutput ("=", variables[i].value);
          variable_counter++;
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
      errOutput ("Incorrect variable\n");
    }
  ptr = strtok (NULL, " ");
  char *equal = ptr;
  if (strcmp (equal, "=") != 0)
    {
      errOutput ("Wrong expression\n");
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

command *goto_commands_array;
command *all_commands_array;

void
basic_translate ()
{
  // counting all instructions
  int instructions_counter = 1;
  while (1)
    {
      char temp[255];
      fgets (temp, 254, input);
      if (feof (input))
        {
          break;
        }
      instructions_counter++;
    }
  basic_counter = instructions_counter;

  rewind (input); // return to the beggining of input file

  all_commands_array = (command *)malloc (sizeof (command) * instructions_counter);
  goto_commands_array = (command *)malloc (sizeof (command) * instructions_counter);

  // fill command str to command structure
  for (int i = 0; i < instructions_counter; i++)
    {
      all_commands_array[i].command = (char *)malloc (sizeof (char) * 255);
      if (!fgets (all_commands_array[i].command, 254, input))
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
    }

  for (int i = 0; i < instructions_counter; i++)
    {
      char *this_command = (char *)malloc (sizeof (char) * 255);
      sprintf (this_command, "%s", all_commands_array[i].command);

      char *ptr = strtok (this_command, " ");
      char *line_num_str = ptr;

      // basic command structure: LINE_NUM COMMAND VARIABLE [GOTO LINE_NUM]
      // check on correct
      if ((strcmp (line_num_str, "0") == 0) || (strcmp (line_num_str, "00") == 0))
        {
          char errMsg[64];
          sprintf (errMsg, "Expected line number on line %d\n", i);
          errOutput (errMsg);
        }

      // fill line number to command structure
      int line_num_int = atoi (line_num_str);
      if (i - 1 >= 0)
        {
          if (line_num_int <= all_commands_array[i - 1].num)
            {
              char errMsg[64];
              sprintf (errMsg, "Wrong line number on line %d\n", i);
              errOutput (errMsg);
            }
        }
      all_commands_array[i].num = line_num_int;

      ptr = strtok (NULL, " ");
      char *cmd = ptr;

      ptr = strtok (NULL, "");
      char *args = ptr;

      all_commands_array[i].address = assembler_counter;

      if (strcmp (cmd, "GOTO") != 0)
        {
          // handling any command except goto
          handle_basic_function (cmd, args);
        }
      else
        {
          // handling new goto
          command new_goto;
          new_goto.num = all_commands_array[i].num;
          new_goto.command = all_commands_array[i].command;
          new_goto.address = all_commands_array[i].address;

          goto_commands_array[i] = new_goto;
          goto_counter++;
          assembler_counter++;
        }
    }
  
  // all goto commands recovery to output file
  for (int i = 0; i <= goto_counter; i++)
    {
      int address = goto_commands_array[i].address;
      char *ptr = strtok (goto_commands_array[i].command, " ");
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
          assemblerOutput ("LOAD", getVariableAddress (x));
          assemblerOutput ("STORE", getVariableAddress (memoryCounter));
          memoryCounter++;
          depth++;
        }
      if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          if (depth < 2)
            {
              errOutput ("Incorrect LET statement\n");
            }
          else
            {
              operand1 = getVariableAddress (memoryCounter - 2);
              operand2 = getVariableAddress (memoryCounter - 1);
              assemblerOutput ("LOAD", operand1);
              switch (x)
                {
                case '+': // summation
                  assemblerOutput ("ADD", operand2);
                  break;
                case '-': // substraction
                  assemblerOutput ("SUB", operand2);
                  break;
                case '/': // division
                  assemblerOutput ("DIVIDE", operand2);
                  break;
                case '*': // multiplication
                  assemblerOutput ("MUL", operand2);
                  break;
                }
              assemblerOutput ("STORE", getVariableAddress (memoryCounter - 2));
              depth--;
              memoryCounter = memoryCounter - 1;
            }
        }
      i++;
    }
  if (depth != 1)
    {
      errOutput ("Incorrect LET statement\n");
    }
  assemblerOutput ("STORE", getVariableAddress (var));
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
      else if (isVariable (operand1))
        {
          operand1Name = operand1[0];
        }
      else
        {
          errOutput ("Wrong statement\n");
        }
    }

  ptr = strtok (NULL, " ");
  char *logicalSign = ptr;
  ptr = strtok (NULL, " ");
  char *operand2 = ptr;

  char operand2Name;
  if (strlen (operand2) > 1 || isdigit (operand2))
    {
      if (atoi (operand2))
        {
          operand2Name = intToConstant (atoi (operand2));
        }
    }
  else
    {
      if (isVariable (operand2))
        {
          operand2Name = operand2[0];
        }
      else
        {
          errOutput ("Wrong statement\n");
        }
    }

  int falsePosition = -1;
  if (logicalSign[0] == '<')
    {
      assemblerOutput ("LOAD", getVariableAddress (operand1Name));
      assemblerOutput ("SUB", getVariableAddress (operand2Name));
      assemblerOutput ("JNEG", assembler_counter + 2);
      falsePosition = assembler_counter;
      assembler_counter++;
    }
  else if (logicalSign[0] == '>')
    {
      assemblerOutput ("LOAD", getVariableAddress (operand2Name));
      assemblerOutput ("SUB", getVariableAddress (operand1Name));
      assemblerOutput ("JNEG", assembler_counter + 2);
      falsePosition = assembler_counter;
      assembler_counter++;
    }
  else if (logicalSign[0] == '=')
    {
      assemblerOutput ("LOAD", getVariableAddress (operand1Name));
      assemblerOutput ("SUB", getVariableAddress (operand2Name));
      assemblerOutput ("JZ", assembler_counter + 2);
      falsePosition = assembler_counter;
      assembler_counter++;
    }

  ptr = strtok (NULL, " ");
  char *cmd = ptr;
  ptr = strtok (NULL, "");
  char *args = ptr;

  if (strcmp (cmd, "IF") == 0)
    {
      errOutput ("Multiple IFs");
    }
  else if (strcmp (cmd, "GOTO") != 0)
    {
      handle_basic_function (cmd, args);
    }
  else
    {
      // handling goto expression in a conditional statement
      goto_counter++;
      command new_goto;
      new_goto.address = assembler_counter;
      char *buff = (char *)malloc (sizeof (char) * 255);
      sprintf (buff, "%d GOTO %s", falsePosition, args);
      new_goto.command = buff;
      goto_commands_array[goto_counter] = new_goto;
      assembler_counter++;
    }
  fprintf (output, "%.2i JUMP %d\n", falsePosition, assembler_counter);
}

int
main (int argc, const char **argv)
{
  if (argc < 3)
    {
      errOutput ("Usage: ./sbt <input_basic_file.sb> <output_assembler_file.sa>\n");
    }
  loadFiles (argv[1], argv[2]);
  basic_translate ();
  fclose (input);
  fclose (output);
  return 0;
}