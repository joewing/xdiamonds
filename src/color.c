/**
 * @file color.c
 * @author Joe Wingbermuehle
 * @date 2004-2006
 *
 * @brief Functions to handle loading colors.
 *
 */

#include "xdiamonds.h"
#include "color.h"
#include "main.h"

/* Map a linear 8-bit RGB space to pixel values. */
static unsigned long *map;

/* Map 8-bit pixel values to a 24-bit linear RGB space. */
static unsigned long *rmap;

static unsigned long redShift;
static unsigned long greenShift;
static unsigned long blueShift;
static unsigned long redMask;
static unsigned long greenMask;
static unsigned long blueMask;

static void ComputeShiftMask(unsigned long maskIn,
   unsigned long *shiftOut, unsigned long *maskOut);

static void GetDirectPixel(XColor *c);
static void GetMappedPixel(XColor *c);

/** Initialize color data. */
void InitializeColors() {

   int x;
   int red, green, blue;
   XColor c;

   /* Determine how to convert between RGB triples and pixels. */
   Assert(rootVisual);
   switch(rootVisual->class) {
   case DirectColor:
   case TrueColor:
      ComputeShiftMask(rootVisual->red_mask, &redShift, &redMask);
      ComputeShiftMask(rootVisual->green_mask, &greenShift, &greenMask);
      ComputeShiftMask(rootVisual->blue_mask, &blueShift, &blueMask);
      map = NULL;
      break;
   default:

      /* Attempt to get 256 colors, pretend it worked. */
      redMask = 0xE0;
      greenMask = 0x1C;
      blueMask = 0x03;
      ComputeShiftMask(redMask, &redShift, &redMask);
      ComputeShiftMask(greenMask, &greenShift, &greenMask);
      ComputeShiftMask(blueMask, &blueShift, &blueMask);
      map = Allocate(sizeof(unsigned long) * 256);

      /* RGB: 3, 3, 2 */
      x = 0;
      for(red = 0; red < 8; red++) {
         for(green = 0; green < 8; green++) {
            for(blue = 0; blue < 4; blue++) {
               c.red = (unsigned short)(74898 * red / 8);
               c.green = (unsigned short)(74898 * green / 8);
               c.blue = (unsigned short)(87381 * blue / 4);
               c.flags = DoRed | DoGreen | DoBlue;
               XAllocColor(display, rootColormap, &c);
               map[x] = c.pixel;
               ++x;
            }
         }
      }

      /* Compute the reverse pixel mapping (pixel -> 24-bit RGB). */
      rmap = Allocate(sizeof(unsigned long) * 256);
      for(x = 0; x < 256; x++) {
         c.pixel = x;
         XQueryColor(display, rootColormap, &c);
         GetDirectPixel(&c);
         rmap[x] = c.pixel;
      }

      break;
   }

}

/** Release color data. */
void DestroyColors() {

   if(map != NULL) {
      XFreeColors(display, rootColormap, map, 256, 0);
      Release(map);
      map = NULL;
      Release(rmap);
      rmap = NULL;
   }

}

/** Compute the mask for computing colors in a linear RGB colormap. */
void ComputeShiftMask(unsigned long maskIn,
   unsigned long *shiftOut, unsigned long *maskOut) {

   int shift;

   Assert(shiftOut);
   Assert(maskOut);

   /* Components are stored in 16 bits.
    * When computing pixels, we'll first shift left 16 bits
    * so to the shift will be an offset from that 32 bit entity.
    * shift = 16 - <shift-to-ones> + <shift-to-zeros>
    */

   shift = 0;
   *maskOut = maskIn;
   while(maskIn && (maskIn & (1 << 31)) == 0) {
      ++shift;
      maskIn <<= 1;
   }
   *shiftOut = shift;

}

/** Compute the RGB components from an index into our RGB colormap. */
void GetColorFromIndex(XColor *c) {

   unsigned long red;
   unsigned long green;
   unsigned long blue;

   Assert(c);

   red = (c->pixel & redMask) << redShift;
   green = (c->pixel & greenMask) << greenShift;
   blue = (c->pixel & blueMask) << blueShift;

   c->red = red >> 16;
   c->green = green >> 16;
   c->blue = blue >> 16;

}

/** Compute the pixel value from RGB components. */
void GetDirectPixel(XColor *c) {

   unsigned long red;
   unsigned long green;
   unsigned long blue;

   Assert(c);

   /* Normalize. */
   red = c->red << 16;
   green = c->green << 16;
   blue = c->blue << 16;

   /* Shift to the correct offsets and mask. */
   red = (red >> redShift) & redMask;
   green = (green >> greenShift) & greenMask;
   blue = (blue >> blueShift) & blueMask;

   /* Combine. */
   c->pixel = red | green | blue;

}

/** Compute the pixel value from RGB components. */
void GetMappedPixel(XColor *c) {

   Assert(c);

   GetDirectPixel(c);
   c->pixel = map[c->pixel];

}

/** Compute the pixel value from RGB components. */
void GetColor(XColor *c) {

   Assert(c);
   Assert(rootVisual);

   switch(rootVisual->class) {
   case DirectColor:
   case TrueColor:
      GetDirectPixel(c);
      return;
   default:
      GetMappedPixel(c);
      return;
   }

}

/** Get the RGB components from a pixel value. */
void GetColorFromPixel(XColor *c) {

   Assert(c);

   /* Convert from a pixel value to a linear RGB space. */
   c->pixel = rmap[c->pixel & 255];

   /* Extract the RGB components from the linear RGB pixel value. */
   GetColorFromIndex(c);

}


/** Get an RGB pixel value from RGB components. */
void GetColorIndex(XColor *c) {

   Assert(c);

   GetDirectPixel(c);

}

