#pragma once

int sc_memoryInit (); // initializes the array of 100 elements

int sc_memorySet (int address,
                  int value); // sets the value of [address] memory unit

int sc_memoryGet (int address, // gets the value of [address] memory unit and
                  int *value); // returns it into value var

int sc_memorySave (
    char *filename); // saves memory into a binary file (write/fwrite)

int sc_memoryLoad (char *filename); // loads RAM from a file (read/fread)

int sc_regInit (void); // inits the register of flags with 0

int sc_regSet (
    int register, // sets the flag register value, #define-s are used for
    int value);   // register numbers, if wrong register number - error

int sc_regGet (int register,
               int *value); // gets the flag value, if wrong register - error

int
sc_commandEncode (int command, // encodes command with a specific number and
                  int operand, // operand, puts the result in value, if wrong
                  int *value); // command or operand - error, value not changes

int sc_commandDecode (
    int value,    // decodes value as a sc command, if decoding is impossible
    int *command, // - sets error command and returns an error
    int *operand);