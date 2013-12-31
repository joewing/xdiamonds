/**
 * @file menu.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "menu.h"
#include "main.h"
#include "font.h"
#include "background.h"

static const char *TITLE_STRING = "XDiamonds v" VERSION;
static const char *AUTHOR_STRING = "by Joe Wingbermuehle";

static const char *menuItems[] = {
   "Start Game",
   "Exit",
   NULL
};

static int maxMenuItemWidth;
static int menuX, menuY;
static int menuSpacing;
static MenuActionType menuAction;

/** Load the menu data. */
void LoadMenu() {
   XFontStruct *font;
   int x;
   int temp;

   font = GetMenuFont();
   if(font) {
      maxMenuItemWidth = XTextWidth(font, menuItems[0],
         strlen(menuItems[0]));
      for(x = 1; menuItems[x]; x++) {
         temp = XTextWidth(font, menuItems[x],
            strlen(menuItems[x]));
         if(temp > maxMenuItemWidth) {
            maxMenuItemWidth = temp;
         }
      }
      menuSpacing = font->ascent + font->descent;
   } else {
      maxMenuItemWidth = 1;
      menuSpacing = 1;
   }

   menuY = 100;
   menuX = WINDOW_WIDTH / 2 - maxMenuItemWidth / 2;

}

/** Unload the menu data. */
void UnloadMenu() {
}

/** Show the menu. */
void ShowMenu() {
   menuAction = MENU_START;
   RedrawMenu();

   XSelectInput(display, mainWindow, KeyPressMask | ButtonPressMask
      | ExposureMask | PointerMotionMask);

}

/** Redraw the menu. */
void RedrawMenu() {

   static const char *menuPointer = ">";

   XFontStruct *font;
   int x, y;
   int len;

   ClearScreen();

   /* Draw title. */
   font = GetTitleFont();
   if(font) {

      XSetFont(display, bufferGC, font->fid);

      len = strlen(TITLE_STRING);
      x = WINDOW_WIDTH / 2 - XTextWidth(font, TITLE_STRING, len) / 2;
      y = font->ascent + font->descent + 10;
      XDrawString(display, buffer, bufferGC, x, y, TITLE_STRING, len);

   }

   /* Draw author. */
   font = GetAuthorFont();
   if(font) {

      XSetFont(display, bufferGC, font->fid);

      len = strlen(AUTHOR_STRING);
      x = WINDOW_WIDTH - XTextWidth(font, AUTHOR_STRING, len) - 10;
      y = WINDOW_HEIGHT - font->ascent - font->descent - 5;
      XDrawString(display, buffer, bufferGC, x, y, AUTHOR_STRING, len);

   }


   /* Draw menu items. */
   font = GetMenuFont();
   if(font) {

      XSetFont(display, bufferGC, font->fid);

      for(x = 0; menuItems[x]; x++) {
         XDrawString(display, buffer, bufferGC, menuX,
            menuY + menuSpacing * x, menuItems[x], strlen(menuItems[x]));
      }

      len = strlen(menuPointer);
      x = menuX - XTextWidth(font, menuPointer, len);
      y = 0;
      switch(menuAction) {
      case MENU_START:
         y = menuY;
         break;
      case MENU_EXIT:
         y = menuY + menuSpacing;
         break;
      default:
         break;
      }

      XDrawString(display, buffer, bufferGC, x, y, menuPointer, len);

   }

   XCopyArea(display, buffer, mainWindow, mainGC, 0, 0,
      WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

}

/** Hide the menu. */
void HideMenu() {
}

/** Move to the next menu item. */
void MoveMenuDown() {
   switch(menuAction) {
   case MENU_START:
      menuAction = MENU_EXIT;
      break;
   default:
      menuAction = MENU_START;
      break;
   }
   RedrawMenu();
}

/** Move to the previous menu item. */
void MoveMenuUp() {
   switch(menuAction) {
   case MENU_EXIT:
      menuAction = MENU_START;
      break;
   default:
      menuAction = MENU_EXIT;
      break;
   }
   RedrawMenu();
}

/** Select a menu item. */
void SelectMenu(int x, int y) {
   int yoffset;

   if(x >= menuX && x < menuX + maxMenuItemWidth) {

      yoffset = menuY - menuSpacing / 2;
      if(y >= yoffset && y < yoffset + menuSpacing) {
         menuAction = MENU_START;
         RedrawMenu();
         return;
      }

      yoffset += menuSpacing;
      if(y >= yoffset && y < yoffset + menuSpacing) {
         menuAction = MENU_EXIT;
         RedrawMenu();
         return;
      }
   }
}

/** Get the current menu action. */
MenuActionType GetMenuAction() {
   return menuAction;
}

