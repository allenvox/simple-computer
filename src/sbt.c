#include "rpn.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLES 26

char *output_filename;
FILE *input = NULL, *output = NULL;

void
loadFiles (const char *input_file, const char *output_file)
{
  if ((input = fopen (input_file, "r")) == NULL)
    {
      errOutput ("Input file does not exist\n");
    }
  output_filename = strdup (output_file);
  output = fopen (output_file, "w");
}

typedef struct command
{
  int basic_line;
  char *command;
  int assembler_line;
} command;

int basic_counter = 0;
int assembler_counter = 0;

void
assemblerOutput (char *cmd, int operand)
{
  fprintf (output, "%.2i %s %.2d\n", assembler_counter, cmd, operand);
  assembler_counter++;
}

typedef struct variable
{
  char name;
  int address;
  int value;
} variable;

variable variables[MAX_VARIABLES * 2];
int variable_counter = 0;

int
isVariable (char name)
{
  if (name >= 'A' && name <= 'Z')
    {
      return 0;
    }
  return 1;
}

int
getVariableAddress (char name)
{
  // loop through all variables, if not found - create new one
  for (int i = 0; i < MAX_VARIABLES * 2; i++)
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

char last_const_alias = 'a';

char
intToConstant (int value)
{
  for (int i = 0; i < MAX_VARIABLES * 2; i++)
    {
      if (variables[i].name == NULL)
        {
          variables[i].name = last_const_alias;
          last_const_alias++;
          variables[i].address = assembler_counter;
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
  return 0;
}

char
preCalcProcessing (char *args)
{
  char *ptr = strtok (args, " =");
  char val;
  sscanf (ptr, "%c", &val); // get variable
  if (isVariable (ptr[0]) != 0)
    {
      errOutput ("Incorrect variable\n");
    }
  
  ptr = strtok (NULL, " ");
  char *equal = ptr; // check equal sign correctness
  if (strcmp (equal, "=") != 0)
    {
      errOutput ("Wrong expression\n");
    }
  
  ptr = strtok (NULL, "");
  char *expression = ptr;
  
  char *assign = (char *)malloc (sizeof (char) * 255);
  int operat = 0, flg = 1, m = 0;
  for (int k = 0; k < (int)strlen (expression); k++)
    {
      if (expression[k] == '-' && flg)
        {
          assign[m] = '0';
          m++;
        }
      flg = 0;
      if (expression[k] == '+' || expression[k] == '-' || expression[k] == '/' || expression[k] == '*')
        {
          operat++;
        }
      if (expression[k] == '+' || expression[k] == '-' || expression[k] == '/' || expression[k] == '*'
          || expression[k] == '(')
        {
          flg = 1;
        }
      assign[m] = expression[k];
      m++;
    }
  
  if (operat == 0) // 0+ перед ним, если перед минусом нет аргумента, то пишем
                   // 0 перед миунсом
    {
      sprintf (expression, "0 + %s", assign);
    }
  else
    {
      sprintf (expression, "%s", assign);
    }
  
  int i = 0, j = 0, pos = 0;
  while (expression[i] != '\n' && expression[i] != '\0')
    {
      if (expression[i] >= '0' && expression[i] <= '9')
        {
          char num[256];
          j = 0;
          num[j] = expression[i];
          j++;
          pos = i;
          expression[i] = ' ';
          i++;
          while (expression[i] >= '0' && expression[i] <= '9')
            {
              num[j] = expression[i];
              j++;
              expression[i] = ' ';
              i++;
            }
          num[j] = '\0';
          expression[pos] = intToConstant (atoi (num));
        }
      i++;
    }
  sprintf (args, "%s", expression); // modify arguments with final expression
  return val;
}

void
INPUT (char var)
{
  if (isVariable (var) != 0)
    {
      printf("%c = %d\n", var, var);
      errOutput ("Wrong variable name\n");
    }
  assemblerOutput ("READ", getVariableAddress (var));
}

void
PRINT (char var)
{
  if (isVariable (var) != 0)
    {
      errOutput ("Wrong variable name.\n");
    }
  assemblerOutput ("WRITE", getVariableAddress (var));
}

int goto_counter = 0;
command *all_commands_array, *goto_commands_array;

int
getGotoDestination (int line_num)
{
  for (int i = 0; i < basic_counter; i++)
    {
      if (all_commands_array[i].basic_line == line_num)
        {
          return all_commands_array[i].assembler_line;
        }
    }
  errOutput ("Memory location specified in GOTO not found\n");
  return 0;
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
LET (char *arguments)
{
  // e.g. args = " B = B * C "
  char expression[255];
  char var = preCalcProcessing (arguments); // put variable to modify in var, expression in args
  // var: ' B ', args: " B * C "
  translateToRPN (arguments, expression); // translate regular expression to RPN expression
  // expression: " BC* "
  parseRPN (expression, var); // assign var to expression
}

void
END ()
{
  assemblerOutput ("HALT", 0);
}

char
*getCorrectIfExpression (char *arguments)
{
  // loop through all args - check if there is space before or after logical sign
  int sign = -1, before = 0, after = 0;
  for (int i = 0; i < (int)strlen (arguments); i++)
    {
      if ((arguments[i] == '>') || (arguments[i] == '<')
          || (arguments[i] == '='))
        {
          sign = i; // sign variable shows logical sign's position
          if (arguments[i - 1] != ' ')
            {
              before = 1;
            }
          if (arguments[i + 1] != ' ')
            {
              after = 1;
            }
          break;
        }
    }
  
  char *expression = (char *)malloc (sizeof (char) * 255);
  if (before == 0 && after == 0) // if sign comes with both spaces between operands
    {
      expression = strtok (arguments, "");
    }
  else // add all needed spaces between operands
    {
      int j = 0;
      for (int i = 0; i < (int)strlen (arguments); i++)
        {
          if (i == sign) // sign is current char
            {
              if (before != 0) // if space not comes before logical sign
                {
                  expression[j] = ' ';
                  j++;
                }
              expression[j] = arguments[i];
              j++;
              if (after != 0) // if space not comes after logical sign
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
  return expression;
}

void handle_basic_function (char *cmd, char *args);

void
IF (char *arguments)
{
  char *expression = getCorrectIfExpression (arguments);
  char *ptr = strtok (expression, " ");
  char *operand1 = ptr;
  char operand1Name;
  if ((strlen (operand1) > 1 || ((operand1[0] >= '0') && (operand1[0] <= '9'))) && (atoi (operand1) != 0))
    {
      operand1Name = intToConstant (atoi (operand1));
    }
  else // if operand is variable
    {
      if (isVariable (operand1[0]) == 0)
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
  if (strlen (operand2) > 1 || ((operand2[0] >= '0') && (operand2[0] <= '9')))
    {
      if (atoi (operand2))
        {
          operand2Name = intToConstant (atoi (operand2));
        }
    }
  else
    {
      if (isVariable (operand2[0]) == 0)
        {
          operand2Name = operand2[0];
        }
      else
        {
          errOutput ("Wrong statement\n");
        }
    }

  int position = -1;
  if (logicalSign[0] == '<')
    {
      assemblerOutput ("LOAD", getVariableAddress (operand2Name));
      assemblerOutput ("SUB", getVariableAddress (operand1Name));
      assemblerOutput ("JNEG", assembler_counter + 2);
      position = assembler_counter;
    }
  else if (logicalSign[0] == '>')
    {
      assemblerOutput ("LOAD", getVariableAddress (operand1Name));
      assemblerOutput ("SUB", getVariableAddress (operand2Name));
      assemblerOutput ("JNEG", assembler_counter + 2);
      position = assembler_counter;
    }
  else if (logicalSign[0] == '=')
    {
      assemblerOutput ("LOAD", getVariableAddress (operand1Name));
      assemblerOutput ("SUB", getVariableAddress (operand2Name));
      assemblerOutput ("JZ", assembler_counter + 2);
      position = assembler_counter;
    }

  ptr = strtok (NULL, " ");
  char *cmd = ptr;
  ptr = strtok (NULL, "");
  char *args = ptr;

  if (strcmp (cmd, "GOTO") == 0)
    {
      // handling goto expression in a conditional statement
      char *buff = (char *)malloc (sizeof (char) * 255);
      sprintf (buff, "%.2i GOTO %s", position, args);
      goto_commands_array[goto_counter].basic_line = position;
      goto_commands_array[goto_counter].assembler_line = assembler_counter;
      goto_commands_array[goto_counter].command = buff;
      goto_counter++;
      assemblerOutput ("JUMP", -1);
    }
  else
    {
      if (strcmp (cmd, "IF") == 0)
        {
          errOutput ("Can't handle multiple if statements");
        }
      handle_basic_function (cmd, args);
    }
  //fprintf (output, "%.2i JUMP %d\n", position, assembler_counter);
}

void
handle_basic_function (char *cmd, char *args)
{
  if (strcmp (cmd, "REM") == 0)
    {
    }
  else if (strcmp (cmd, "INPUT") == 0)
    {
      INPUT (args[0]);
    }
  else if (strcmp (cmd, "PRINT") == 0)
    {
      PRINT (args[0]);
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
      sprintf (errMsg, "'%s' is not a valid command\n", cmd);
      errOutput (errMsg);
    }
}

void
replaceLine (int dstline, char *new_line)
{
  FILE *fPtr = fopen (output_filename, "r");
  FILE *fTemp = fopen ("out.tmp", "w");
  if (fPtr == NULL || fTemp == NULL)
    {
      errOutput ("Can't open a file");
    }
  char buff[64];
  int current_line = -1; // since 0 assembly line = 1 txt line
  while ((fgets (buff, 64, fPtr)) != NULL)
    {
      current_line++;
      if (current_line == dstline)
        {
          fputs (new_line, fTemp);
        }
      else
        {
          fputs (buff, fTemp);
        }
    }
  fclose (fPtr);
  fclose (fTemp);
  remove (output_filename);
  rename ("out.tmp", output_filename);
}

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

      // check line num correctness
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
          if (line_num_int <= all_commands_array[i - 1].basic_line)
            {
              char errMsg[64];
              sprintf (errMsg, "Wrong line number on line %d\n", i);
              errOutput (errMsg);
            }
        }
      all_commands_array[i].basic_line = line_num_int;

      ptr = strtok (NULL, " ");
      char *cmd = ptr;

      ptr = strtok (NULL, "");
      char *args = ptr;

      all_commands_array[i].assembler_line = assembler_counter;

      if (strcmp (cmd, "GOTO") != 0)
        {
          handle_basic_function (cmd, args);
        }
      else
        {
          // handling new direct goto
          goto_commands_array[goto_counter].basic_line = all_commands_array[i].basic_line;
          goto_commands_array[goto_counter].command = all_commands_array[i].command;
          goto_commands_array[goto_counter].assembler_line = all_commands_array[i].assembler_line;
          goto_counter++;
          assemblerOutput ("JUMP", -1);
        }
    }
  fclose (output);
  
  // recover all goto commands
  for (int i = 1; i <= goto_counter; i++)
    {
      command current_goto = goto_commands_array[i - 1];

      int assembler_line_to_change = current_goto.assembler_line;
      char *cmd = strdup (current_goto.command);
      char *ptr = strtok (cmd, " ");
      ptr = strtok (NULL, " ");
      ptr = strtok (NULL, "");
      int dst_basic_line = atoi (ptr);

      char new_goto_line[64];
      sprintf (new_goto_line, "%.2i JUMP %.2d\n", assembler_line_to_change, getGotoDestination (dst_basic_line));
      replaceLine (assembler_line_to_change, new_goto_line);
    }
}

int
main (int argc, const char **argv)
{
  if (argc < 3)
    {
      errOutput ("Usage: ./sbt <IN_program.sb> <OUT_assembler_file.sa>\n");
    }
  loadFiles (argv[1], argv[2]);
  basic_translate ();
  fclose (input);
  return 0;
}