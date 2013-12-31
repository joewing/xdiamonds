/**
 * @file background.h
 * @author Joe Wingbermuehle
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

/** Load the background image. */
void LoadBackground();

/** Unload the background image. */
void UnloadBackground();

/** Clear the whole screen. */
void ClearScreen();

/** Clear an area of the screen.
 * @param xc The starting x-coordinate.
 * @param yc The starting y-coordinate.
 * @param width The width of the area to clear.
 * @param height The height of the area to clear.
 */
void ClearArea(int xc, int yc, int width, int height);

#endif /* BACKGROUND_H */

