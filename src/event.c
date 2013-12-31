/**
 * @file event.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "event.h"
#include "main.h"
#include "menu.h"
#include "level.h"
#include "ball.h"
#include "score.h"

static void DoStartDelay();
static void EventLoop();
static int DoAction();
static void WinLoop();

static void HandleExposeEvent(XExposeEvent *event);
static void HandleKeyPressEvent(XKeyEvent *event);
static void HandleKeyReleaseEvent(XKeyEvent *event);
static void HandleButtonPressEvent();

static KeyCode keyRight, keyLeft, keyUp, keyDown;
static KeyCode keySpace, keyEnter, keyBackspace, keyESC;
static KeyCode keyJ, keyK, keyL, keyI;

static int moveRight, moveLeft;
static int isPaused;

/** The main event loop. */
void MainLoop() {

   XEvent event;

   keyRight = XKeysymToKeycode(display, XK_Right);
   keyLeft = XKeysymToKeycode(display, XK_Left);
   keyUp = XKeysymToKeycode(display, XK_Up);
   keyDown = XKeysymToKeycode(display, XK_Down);
   keyESC = XKeysymToKeycode(display, XK_Escape);
   keySpace = XKeysymToKeycode(display, XK_space);
   keyEnter = XKeysymToKeycode(display, XK_Return);
   keyBackspace = XKeysymToKeycode(display, XK_BackSpace);

   keyJ = XKeysymToKeycode(display, XK_J);
   keyK = XKeysymToKeycode(display, XK_K);
   keyL = XKeysymToKeycode(display, XK_L);
   keyI = XKeysymToKeycode(display, XK_I);

   ShowMenu();

   for(;;) {
      while(XPending(display) > 0) {
         XNextEvent(display, &event);
         switch(event.type) {
         case Expose:
            RedrawMenu();
            break;
         case KeyPress:
            if(event.xkey.keycode == keyDown
               || event.xkey.keycode == keyK) {
               MoveMenuDown();
            } else if(event.xkey.keycode == keyUp
               || event.xkey.keycode == keyI) {
               MoveMenuUp();
            } else if(event.xkey.keycode == keySpace
               || event.xkey.keycode == keyEnter) {
               if(!DoAction()) {
                  return;
               }
            }
            break;
         case ButtonPress:
            if(!DoAction()) {
               return;
            }
            break;
         case MotionNotify:
            SelectMenu(event.xmotion.x, event.xmotion.y);
            break;
         default:
            break;
         }
      }
   }

}

/** Process a menu action. */
int DoAction() {
   switch(GetMenuAction()) {
   case MENU_START:
      HideMenu();
      EventLoop();
      ShowMenu();
      return 1;
   case MENU_EXIT:
      HideMenu();
      return 0;
   default:
      return 1;
   }
}

/** Delay after drawing the game screen. */
void DoStartDelay() {

   struct timeval last, current;
   unsigned long elapsedTime;
   XEvent event;

   gettimeofday(&last, NULL);

   while(!shouldExit) {
      while(XPending(display) > 0) {
         XNextEvent(display, &event);
         switch(event.type) {
         case Expose:
            HandleExposeEvent(&event.xexpose);
            break;
         case KeyPress:
            if(event.xkey.keycode == keyESC) {
               shouldExit = 1;
            }
            break;
         default:
            break;
         }
      }

      gettimeofday(&current, NULL);

      elapsedTime = (current.tv_sec - last.tv_sec) * 1000000;
      elapsedTime += current.tv_usec - last.tv_usec;
      if(elapsedTime >= START_DELAY) {
         break;
      }
   }
   moveRight = 0;
   moveLeft = 0;
   isPaused = 0;

}

/** Game event loop. */
void EventLoop() {

   struct timeval last, current;
   unsigned long elapsedTime;
   int repeat;
   XEvent event;

   SetFirstLevel();

   XSelectInput(display, mainWindow, KeyPressMask | KeyReleaseMask
      | ButtonPressMask | ButtonReleaseMask | ExposureMask);

   shouldExit = 0;

   DoStartDelay();

   gettimeofday(&last, NULL);

   while(!shouldExit) {

      if(didWin) {
         WinLoop();
         NextLevel();
         moveRight = 0;
         moveLeft = 0;
         gettimeofday(&last, NULL);
      }
      if(didLose) {
         Redraw();
         DoStartDelay();
         break;
      }

      while(XPending(display) > 0) {
         XNextEvent(display, &event);
         switch(event.type) {
         case Expose:
            HandleExposeEvent(&event.xexpose);
            break;
         case KeyPress:
            HandleKeyPressEvent(&event.xkey);
            break;
         case KeyRelease:
            HandleKeyReleaseEvent(&event.xkey);
            break;
         case ButtonPress:
            HandleButtonPressEvent();
            break;
         default:
            break;
         }
      }

      if(isPaused) {
         gettimeofday(&last, NULL);
         continue;
      }

      gettimeofday(&current, NULL);
      elapsedTime = (current.tv_sec - last.tv_sec) * 1000000;
      elapsedTime += current.tv_usec - last.tv_usec;

      if(elapsedTime >= SPEED) {
         last = current;

         EraseBall();
         repeat = (int)(((float)elapsedTime / SPEED) + 0.5);
         do {
            if(moveRight && !moveLeft) {
               MoveBallRight();
            } else if(moveLeft && !moveRight) {
               MoveBallLeft();
            }
            MoveBall();
            --repeat;
         } while(repeat);
         DrawBall();

         Redraw();

         if(didDie) {
            didDie = 0;
            DoStartDelay();
            gettimeofday(&last, NULL);
         }

      }

   }
}

/** Event loop for a win. */
void WinLoop() {
   struct timeval last, current;
   unsigned long elapsedTime;
   XEvent event;
   int count;

   if(!GetBonus()) {
      return;
   }

   gettimeofday(&last, NULL);
   count = 64;
   while(!shouldExit) {

      while(XPending(display) > 0) {
         XNextEvent(display, &event);
         switch(event.type) {
         case Expose:
            HandleExposeEvent(&event.xexpose);
            break;
         case KeyPress:
            if(event.xkey.keycode == keyESC) {
               shouldExit = 1;
            }
            break;
         default:
            break;
         }
      }

      gettimeofday(&current, NULL);
      elapsedTime = (current.tv_sec - last.tv_sec) * 1000000;
      elapsedTime += current.tv_usec - last.tv_usec;

      if(elapsedTime >= COUNTDOWN_SPEED) {
         last = current;
         UpdateBonusCountdown();
         --count;
         if(count <= 0) {
            break;
         }
      }
   }
}

/** Handle an expose event. */
void HandleExposeEvent(XExposeEvent *event) {
   Redraw();
}

/** Handle a key press event. */
void HandleKeyPressEvent(XKeyEvent *event) {
   if(event->keycode == keyRight || event->keycode == keyL) {
      moveRight = 1;
   } else if(event->keycode == keyLeft || event->keycode == keyJ) {
      moveLeft = 1;
   } else if(event->keycode == keyESC) {
      shouldExit = 1;
   } else if(event->keycode == keySpace) {
      if(isPaused) {
         isPaused = 0;
      } else {
         isPaused = 1;
      }
      SetPaused(isPaused);
   } else if(event->keycode == keyBackspace) {
      EraseBall();
      UpdateLives(-1);
      SetBallForRestart();
      didDie = 1;
   }
}

/** Handle a key release event. */
void HandleKeyReleaseEvent(XKeyEvent *event) {
   if(event->keycode == keyRight || event->keycode == keyL) {
      moveRight = 0;
   } else if(event->keycode == keyLeft || event->keycode == keyJ) {
      moveLeft = 0;
   }
}

/** Handle a mouse button press event. */
void HandleButtonPressEvent() {
}

