/**
 * @file main.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "main.h"
#include "color.h"
#include "font.h"
#include "background.h"
#include "score.h"
#include "block.h"
#include "menu.h"
#include "ball.h"
#include "event.h"

Display *display;
Window mainWindow, buffer;
GC mainGC, bufferGC;
Visual *rootVisual;
int rootScreen;
Colormap rootColormap;
int rootDepth;

int shouldExit;
int didDie;
int didWin;
int didLose;

unsigned long foreground;
unsigned long background;

/** Main entry point. */
int main(int argc, char *argv[]) {

   XColor temp;

   StartDebug();

   display = XOpenDisplay(NULL);
   if(!display) {
      printf("error: could not connect to X server\n");
      StopDebug();
      return 1;
   }

   shouldExit = 0;

   rootScreen = DefaultScreen(display);
   rootDepth = DefaultDepth(display, rootScreen);
   rootColormap = DefaultColormap(display, rootScreen);
   rootVisual = DefaultVisual(display, rootScreen);

   mainWindow = XCreateSimpleWindow(display,
      RootWindow(display, DefaultScreen(display)),
      0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0);
   XStoreName(display, mainWindow, "xdiamonds v" VERSION);

   XMapWindow(display, mainWindow);

   mainGC = XCreateGC(display, mainWindow, 0, NULL);

   buffer = XCreatePixmap(display, mainWindow, WINDOW_WIDTH,
      WINDOW_HEIGHT, DefaultDepth(display, DefaultScreen(display)));

   bufferGC = XCreateGC(display, buffer, 0, NULL);

   InitializeColors();

   temp.red = BACKGROUND_RED * 257;
   temp.green = BACKGROUND_GREEN * 257;
   temp.blue = BACKGROUND_BLUE * 257;
   GetColor(&temp);
   background = temp.pixel;

   temp.red = FOREGROUND_RED * 257;
   temp.green = FOREGROUND_GREEN * 257;
   temp.blue = FOREGROUND_BLUE * 257;
   GetColor(&temp);
   foreground = temp.pixel;

   LoadFonts();
   LoadBackground();
   LoadScore();
   LoadBlocks();
   LoadBall();
   LoadMenu();

   MainLoop();

   UnloadMenu();
   UnloadBlocks();
   UnloadScore();
   UnloadBackground();
   UnloadBall();
   UnloadFonts();
   DestroyColors();

   XDestroyWindow(display, mainWindow);

   XCloseDisplay(display);

   StopDebug();
   return 0;

}

