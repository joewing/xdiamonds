/**
 * @file font.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "font.h"
#include "main.h"

static const char *MENU_FONT = "-*-helvetica-*-r-*-*-24-*-*-*-*-*-*-*";
static const char *SCORE_FONT = "-*-courier-*-r-*-*-18-*-*-*-*-*-*-*";
static const char *TITLE_FONT = "-*-helvetica-*-r-*-*-30-*-*-*-*-*-*-*";
static const char *AUTHOR_FONT = "-*-helvetica-*-r-*-*-16-*-*-*-*-*-*-*";
static const char *DEFAULT_FONT = "-*-courier-*-r-*-*-14-*-*-*-*-*-*-*";

static XFontStruct *scoreFont = NULL;
static XFontStruct *menuFont = NULL;
static XFontStruct *titleFont = NULL;
static XFontStruct *authorFont = NULL;

/** Load font data. */
void LoadFonts() {

   menuFont = XLoadQueryFont(display, MENU_FONT);
   if(!menuFont) {
      Warning("could not load font: %s", MENU_FONT);
      menuFont = XLoadQueryFont(display, DEFAULT_FONT);
   }

   scoreFont = XLoadQueryFont(display, SCORE_FONT);
   if(!scoreFont) {
      Warning("could not load font: %s", SCORE_FONT);
      scoreFont = XLoadQueryFont(display, DEFAULT_FONT);
   }

   titleFont = XLoadQueryFont(display, TITLE_FONT);
   if(!titleFont) {
      Warning("could not load font: %s", TITLE_FONT);
      titleFont = XLoadQueryFont(display, DEFAULT_FONT);
   }

   authorFont = XLoadQueryFont(display, AUTHOR_FONT);
   if(!authorFont) {
      Warning("could not load font: %s", AUTHOR_FONT);
      authorFont = XLoadQueryFont(display, DEFAULT_FONT);
   }

}

/** Unload font data. */
void UnloadFonts() {

   if(menuFont) {
      XFreeFont(display, menuFont);
   }

   if(scoreFont) {
      XFreeFont(display, scoreFont);
   }

   if(titleFont) {
      XFreeFont(display, titleFont);
   }

   if(authorFont) {
      XFreeFont(display, authorFont);
   }

}

/** Get the menu font. */
XFontStruct *GetMenuFont() {
   return menuFont;
}

/** Get the title font. */
XFontStruct *GetTitleFont() {
   return titleFont;
}

/** Get the author font. */
XFontStruct *GetAuthorFont() {
   return authorFont;
}

/** Get the score font. */
XFontStruct *GetScoreFont() {
   return scoreFont;
}

