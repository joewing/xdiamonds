/**
 * @file xdiamonds.h
 * @author Joe Wingbermuehle
 */

#ifndef XDIAMONDS_H
#define XDIAMONDS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#define VERSION "0.2"

#define LEVEL_WIDTH 8
#define LEVEL_HEIGHT 10
#define BLOCK_WIDTH 64
#define BLOCK_HEIGHT 32
#define RHS_WIDTH 128
#define WINDOW_WIDTH ((LEVEL_WIDTH * BLOCK_WIDTH) + RHS_WIDTH)
#define WINDOW_HEIGHT (LEVEL_HEIGHT * BLOCK_HEIGHT)

#define SPEED 20000
#define COUNTDOWN_SPEED 100000
#define START_DELAY 1000000
#define BALL_DELTA_X 5
#define BALL_DELTA_Y 4

#define BACKGROUND_RED   255
#define BACKGROUND_GREEN 255
#define BACKGROUND_BLUE  255

#define FOREGROUND_RED   0
#define FOREGROUND_GREEN 0
#define FOREGROUND_BLUE  0

#include "debug.h"
#include "error.h"

#endif /* XDIAMONDS_H */

