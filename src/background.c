/**
 * @file background.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "background.h"
#include "main.h"
#include "image.h"

#include "images/bg.xpm"

static ImageType *bgImage;

/** Load the background image. */
void LoadBackground() {
   bgImage = LoadImage(bg_xpm);
}

/** Unload the background image. */
void UnloadBackground() {
   if(bgImage) {
      DestroyImage(bgImage);
   }
}

/** Clear the whole screen. */
void ClearScreen() {
   ClearArea(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

/** Clear an area of the screen. */
void ClearArea(int xc, int yc, int width, int height) {

   XRectangle rect;
   int startx, starty;
   int x, y;

   /* If for some reason the background couldn't be loaded, return. */
   if(!bgImage) {
      return;
   }

   /* Set up the clip mask. */
   rect.x = xc;
   rect.y = yc;
   rect.width = width;
   rect.height = height;
   XSetClipRectangles(display, bufferGC, 0, 0, &rect, 1, Unsorted);

   /* Tile the background image. */
   starty = yc - (yc % bgImage->image->height);
   startx = xc - (xc % bgImage->image->width);
   for(y = starty; y < yc + height; y += bgImage->image->height) {
      for(x = startx; x < xc + width; x += bgImage->image->width) {
         XPutImage(display, buffer, bufferGC, bgImage->image, 0, 0, x, y,
            bgImage->image->width, bgImage->image->height);
      }
   }

   /* Clear the clip mask. */
   XSetClipMask(display, bufferGC, None);

}

