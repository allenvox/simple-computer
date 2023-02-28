#pragma once

#include <sys/param.h>

#define BC_X 15
#define BC_START 8
#define BC_STEP 8
#define X_START 2
#define Y_START 3
#define Y_STEP 6

int g_memorybox ();
int g_accumbox ();
int g_counterbox ();
int g_operationbox ();
int g_flagbox ();
int g_bcbox (int *big);
int g_flags (char **val);
int g_static ();