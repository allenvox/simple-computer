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

int g_writeflags (char **val);
int g_drawborders (void);
int g_drawaccumbox (void);
int g_drawflagbox (void);
int g_drawbcbox (void);
int g_loadmemory (void);
int g_savememory (void);
int g_setmemory (int x, int y);
int g_getunit (int address, char **buff);
int g_highlightmemory (int x, int y);
int g_drawoperationbox (void);
int g_drawcounterbox (void);
int g_drawmemorybox (void);
int g_drawboxes ();
int g_interface ();