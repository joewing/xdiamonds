/**
 * @file image.h
 * @author Joe Wingbermuehle
 */

#ifndef IMAGE_H
#define IMAGE_H

typedef struct ImageType {
	XImage *image;
	Pixmap mask;
} ImageType;

/** Load an image. */
ImageType *LoadImage(char **data);

/** Draw an image.
 * @param d The drawing on which to draw.
 * @param gc The graphics context to use for drawing.
 * @param image The image to draw.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 */
void DrawImage(Drawable d, GC gc, ImageType *image, int x, int y);

/** Destroy an image.
 * @param image The image to destroy.
 */
void DestroyImage(ImageType *image);

#endif /* IMAGE_H */

