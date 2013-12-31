/**
 * @file font.h
 * @author Joe Wingbermuehle
 */

#ifndef FONT_H
#define FONT_H

/** Load font data. */
void LoadFonts();

/** Unload font data. */
void UnloadFonts();

/** Get the font to use for the title.
 * @return The font.
 */
XFontStruct *GetTitleFont();

/** Get the font to use for the author.
 * @return The font.
 */
XFontStruct *GetAuthorFont();

/** Get the font to use for the menu.
 * @return The font.
 */
XFontStruct *GetMenuFont();

/** Get the font to use for displaying the score.
 * @return The font.
 */
XFontStruct *GetScoreFont();

#endif /* FONT_H */

