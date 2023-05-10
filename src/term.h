#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define CLEAR "\E[H\E[2J"

enum colors
{
  LIGHT_GREY = 247,
  GREY = 237,
  BLACK = 16,
  RED = 124,
  GREEN = 41,
  BLUE = 20,
  YELLOW = 184,
};

int mt_clrscr (void); // clears screen and moves cursor to upper left corner
int mt_gotoXY (
    int x,
    int y); // moves cursor to the entered coordinates (x, y) = (row, col)
int mt_getscreensize (
    int *rows, int *cols); // gets terminal screen size (num of rows and cols)
int mt_setfgcolor (enum colors color); // sets a background color for all rows
                                       // and cols, entire terminal
int mt_setbgcolor (
    enum colors color); // sets a background color only for upcoming characters
int mt_readtext (char *text, int size);
int mt_printtext (char *text);