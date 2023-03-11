#pragma once

#include "bc.h"
#include "msc.h"
#include "readkey.h"
#include "term.h"
#include "sig.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#define BC_X 15
#define BC_START 8
#define BC_STEP 8
#define X_START 2
#define Y_START 3
#define Y_STEP 6

int g_memorybox (void);
int g_accumbox (void);
int g_counterbox (void);
int g_operationbox (void);
int g_flagbox (void);
int g_bcbox (void);
int g_flags (char **val);
int g_static (void);
int g_loadmemory (void);
int g_init (void);
int g_savememory (void);
int g_printmem (int *x, int *y);
int g_interface (void);