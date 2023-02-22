#pragma once

#include <sys/param.h>

#define BC_X 15
#define BC_START 8
#define BC_STEP 8
#define X_START 2
#define Y_START 3
#define Y_STEP 6

void g_memorybox ();
void g_accumbox ();
void g_counterbox ();
void g_operationbox ();
void g_flagbox ();
void g_bcbox (int *big);
char *g_flags ();
void g_static ();
void g_printbc (int x, int y);