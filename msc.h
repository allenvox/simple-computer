#pragma once

int sc_memoryInit(); // initializes the array of 100 elements
int sc_memorySet(int address, int value); // sets the value of [address] memory unit
int sc_memoryGet(int address, int *value); // gets the value of [address] memory unit and returns it into value var
int sc_memorySave (char *filename); // saves memory into a binary file (write/fwrite)
int sc_memoryLoad(char *filename); // loads RAM from a file (read/fread)
int sc_regInit (void); // inits the register of flags with 0
int sc_regSet (int register, int value); // sets the flag register value, #define-s are used for register numbers, if wrong register number - error
int sc_regGet (int register, int *value); // gets the flag value, if wrong register - error
int sc_commandEncode(int command, int operand, int *value); // encodes command with a specific number and operand, puts the result in value, if wrong command or operand - error, value not changes
int sc_commandDecode(int value, int *command, int *operand); // decodes value as a sc command, if decoding is impossible - sets error command and returns an error