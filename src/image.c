/**
 * @file image.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "image.h"
#include "main.h"

#include <X11/xpm.h>

static ImageType *CreateImageFromXImages(XImage *image, XImage *shape);

/** Load an image from the specified XPM data. */
ImageType *LoadImage(char **data) {

   ImageType *result;
   XImage *image;
   XImage *shape;
   XpmAttributes attr;
   int rc;

   Assert(data);

   result = NULL;
   attr.valuemask = XpmExactColors | XpmRGBCloseness | XpmCloseness;
   attr.exactColors = 0;
   attr.closeness = 65535;
   attr.red_closeness = 255;
   attr.green_closeness = 255;
   attr.blue_closeness = 255;
   rc = XpmCreateImageFromData(display, data, &image, &shape, &attr);
   if(rc == XpmSuccess) {
      result = CreateImageFromXImages(image, shape);
      XDestroyImage(image);
      if(shape) {
         XDestroyImage(shape);
      }
   } else {
      Warning("failed to load image: %d", rc);
   }

   return result;

}

/** Create an image from XImages giving color and shape information. */
ImageType *CreateImageFromXImages(XImage *image, XImage *shape) {

   ImageType *result;
   XColor color;
   int x, y;
   GC maskGC;

   result = Allocate(sizeof(ImageType));
   result->image = XCreateImage(display, rootVisual, rootDepth,
      ZPixmap, 0, NULL, image->width, image->height, 32, 0);
   if(!result->image) {
      Warning("creation of XImage failed");
      Release(result);
      return NULL;
   }
   result->image->data = Allocate(4 * image->width * image->height);

   result->mask = XCreatePixmap(display, mainWindow,
      image->width, image->height, 1);
   maskGC = XCreateGC(display, result->mask, 0, NULL);
   XSetForeground(display, maskGC, 0);
   XFillRectangle(display, result->mask, maskGC, 0, 0,
      image->width, image->height);
   XSetForeground(display, maskGC, 1);

   for(y = 0; y < image->height; y++) {
      for(x = 0; x < image->width; x++) {

         color.pixel = XGetPixel(image, x, y);
         XPutPixel(result->image, x, y, color.pixel);

         if(!shape || XGetPixel(shape, x, y)) {
            XDrawPoint(display, result->mask, maskGC, x, y);
         }

      }
   }

   return result;

}

/** Draw an image. */
void DrawImage(Drawable d, GC gc, ImageType *image, int x, int y) {

   if(!image || !image->image) {
      return;
   }

   XSetClipOrigin(display, gc, x, y);
   XSetClipMask(display, gc, image->mask);

   XPutImage(display, d, gc, image->image, 0, 0, x, y,
      image->image->width, image->image->height);

   XSetClipMask(display, gc, None);

}

/** Destroy an image. */
void DestroyImage(ImageType *image) {
   if(image) {
      if(image->image) {
         Release(image->image->data);
         image->image->data = NULL;
         XDestroyImage(image->image);
      }
      XFreePixmap(display, image->mask);
      Release(image);
   }
}

