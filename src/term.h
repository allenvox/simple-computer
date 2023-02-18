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
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  UNKNOWN,
  DEFAULT
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
