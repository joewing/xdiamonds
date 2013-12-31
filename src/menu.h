/**
 * @file menu.h
 * @author Joe Wingbermuehle
 */

#ifndef MENU_H
#define MENU_H

/** Menu items. */
typedef enum {
	MENU_NONE           = 0,
	MENU_START          = 1,
	MENU_EXIT           = 2
} MenuActionType;

/** Load the menu. */
void LoadMenu();

/** Unload the menu. */
void UnloadMenu();

/** Show the menu screen. */
void ShowMenu();

/** Redraw the menu screen. */
void RedrawMenu();

/** Hide the menu screen. */
void HideMenu();

/** Move to the next item on the menu. */
void MoveMenuDown();

/** Move to the previous item on the menu. */
void MoveMenuUp();

/** Select a specific pixel location on the menu. */
void SelectMenu(int x, int y);

/** Get the current menu action. */
MenuActionType GetMenuAction();

#endif /* MENU_H */

