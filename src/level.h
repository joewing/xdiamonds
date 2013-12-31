/**
 * @file level.h
 * @author Joe Wingbermuehle
 */

#ifndef LEVEL_H
#define LEVEL_H

typedef struct {
	int x, y;
	char data[LEVEL_WIDTH * LEVEL_HEIGHT];
} LevelType;

/** Level data for the current level. */
extern LevelType currentLevel;

/** Load the first level. */
void SetFirstLevel();

/** Load the next level. */
void NextLevel();

/** Mark the level as updated.
 * This should be called any time level data changes so that
 * the level is re-rendered.
 */
void SetUpdated();

/** Update a block on the current level.
 * @param x The level x-coordinate.
 * @param y The level y-coordinate.
 */
void UpdateBlock(int x, int y);

/** Redraw the current level. */
void Redraw();

/** Get the current level. */
int GetLevel();

#endif /* LEVEL_H */

