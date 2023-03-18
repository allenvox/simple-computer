#pragma once

#include "alu.h"
#include "msc.h"
#include "tui.h"
#include "term.h"
#include <string.h>

int READ (int operand);
int WRITE (int operand);
int LOAD (int operand);
int STORE (int operand);
int JUMP (int operand);
int JNEG (int operand);
int JZ (int operand);
int HALT (void);
int JNS (int operand);
int CU (void);