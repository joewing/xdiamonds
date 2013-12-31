/**
 * @file main.h
 * @author Joe Wingbermuehle
 */

#ifndef MAIN_H
#define MAIN_H

extern Display *display;
extern Window mainWindow, buffer;
extern GC mainGC, bufferGC;
extern Visual *rootVisual;
extern Colormap rootColormap;
extern int rootDepth;
extern int rootScreen;

extern int shouldExit;
extern int didDie;
extern int didWin;
extern int didLose;

extern unsigned long foreground;
extern unsigned long background;

#endif /* MAIN_H */

