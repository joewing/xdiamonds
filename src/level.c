/**
 * @file level.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "level.h"
#include "main.h"
#include "score.h"
#include "block.h"
#include "ball.h"
#include "background.h"

static LevelType LEVELS[] = {
   { 0, 0,
      {   0, 0, 3, 4, 4, 3, 0, 0,
         0, 0, 8, 8, 8, 8, 0, 0,
         0, 5, 4, 4, 4, 4, 9, 0,
         0, 1, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 2, 2, 0, 0, 0,
         0, 0, 0, 2, 2, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 1, 0,
         0, 9, 4, 4, 4, 4, 5, 0,
         0, 0, 8, 8, 8, 8, 0, 0,
         0, 0, 3, 4, 4, 3, 0, 0
      }
   },

   { 3, 6,
      {   0, 0, 0, 0, 0, 0, 0, 0,
         5, 0, 0, 0, 0, 0, 0, 7,
         2, 6, 0, 0, 0, 0, 6, 2,
         2, 6, 4, 0, 0, 4, 6, 2,
         3, 6, 4, 1, 1, 4, 6, 3,
         3, 6, 4, 1, 1, 4, 6, 3,
         2, 6, 4, 0, 0, 4, 6, 2,
         2, 6, 0, 0, 0, 0, 6, 2,
         7, 0, 0, 0, 0, 0, 0, 5,
         0, 0, 0, 0, 0, 0, 0, 0
      }
   },
   { 4, 5,
      {   9, 0, 2, 14, 14, 2, 0, 8,
         13, 0, 14, 2, 2, 14, 0, 12,
         0, 0, 2, 2, 2, 2, 0, 0,
         0, 0, 14, 2, 2, 14, 0, 0,
         6, 6, 14, 2, 2, 14, 4, 4,
         6, 6, 0, 0, 0, 0, 4, 4,
         6, 7, 0, 0, 0, 0, 5, 4,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         1, 1, 3, 14, 14, 3, 1, 1
      }
   },
   { 0, 6,
      {   5, 14, 2, 14, 14, 4, 4, 4,
         8, 14, 2, 14, 14, 4, 4, 4,
         0, 0, 0, 4, 4, 0, 0, 0,
         0, 0, 0, 14, 14, 0, 0, 0,
         14, 8, 14, 14, 14, 14, 14, 14,
         14, 8, 14, 3, 14, 14, 14, 14,
         0, 0, 0, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         14, 1, 1, 14, 9, 1, 1, 1,
         14, 2, 2, 14, 14, 2, 2, 2
      }
   },
   { 7, 7,
      {   0, 1, 4, 0, 0, 2, 2, 2,
         0, 0, 1, 4, 0, 0, 0, 0,
         1, 0, 0, 1, 5, 0, 0, 0,
         8, 1, 0, 0, 1, 8, 0, 0,
         0, 3, 1, 0, 0, 1, 3, 0,
         0, 0, 4, 1, 0, 0, 1, 4,
         0, 0, 0, 9, 1, 0, 0, 1,
         0, 0, 0, 0, 8, 1, 0, 0,
         2, 2, 2, 0, 0, 8, 1, 0,
         14, 14, 14, 14, 14, 14, 8, 14
      }
   },
   { 5, 4,
      {   3, 9, 3, 2, 2, 3, 5, 3,
         1, 1, 14, 2, 2, 14, 1, 1,
         0, 0, 14, 2, 2, 14, 0, 0,
         0, 0, 14, 4, 4, 14, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         1, 1, 1, 1, 1, 1, 1, 1,
         8, 8, 8, 3, 3, 8, 8, 8,
         8, 3, 8, 3, 3, 8, 3, 8
      }
   },
   { 0, 3,
      {   2, 2, 2, 14, 0, 0, 1, 5,
         2, 2, 2, 14, 0, 0, 1, 1,
         6, 14, 14, 14, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         14, 14, 14, 14, 14, 14, 14, 0,
         14, 14, 14, 14, 14, 14, 14, 0,
         7, 0, 0, 0, 0, 0, 0, 0,
         3, 0, 0, 4, 8, 4, 8, 3,
         9, 0, 0, 4, 8, 4, 8, 3
      }
   },
   { 0, 1,
      {   14, 3, 14, 3, 14, 14, 9, 3,
         0, 0, 1, 0, 1, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         14, 1, 0, 1, 0, 1, 0, 0,
         3, 14, 14, 5, 3, 14, 14, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         3, 14, 4, 4, 8, 8, 14, 3,
         3, 14, 4, 3, 3, 8, 14, 3,
         3, 14, 4, 2, 2, 8, 14, 3
      }
   },
   { 0, 8,
      {   12, 9, 14, 0, 0, 0, 0, 3,
         0, 0, 1, 0, 0, 0, 0, 4,
         0, 0, 1, 0, 14, 14, 14, 14,
         0, 0, 1, 0, 14, 0, 8, 8,
         0, 0, 1, 0, 13, 0, 8, 5,
         1, 1, 14, 0, 14, 0, 8, 8,
         2, 2, 3, 0, 14, 14, 14, 14,
         14, 14, 14, 0, 0, 0, 0, 4,
         0, 0, 0, 0, 0, 0, 0, 3,
         0, 0, 0, 0, 0, 0, 0, 4
      }
   },
   { 0, 0,
      {   0, 0, 6, 0, 0, 13, 0, 0,
         0, 8, 14, 0, 0, 14, 4, 0,
         0, 8, 14, 0, 0, 14, 4, 0,
         0, 1, 5, 0, 0, 9, 0, 0,
         0, 1, 3, 2, 2, 3, 0, 0,
         0, 1, 3, 2, 2, 3, 0, 0,
         0, 1, 9, 7, 7, 5, 0, 0,
         0, 4, 14, 0, 0, 14, 8, 0,
         0, 4, 14, 0, 0, 14, 8, 0,
         0, 0, 8, 0, 0, 12, 0, 0
      }
   }

};

static const int LEVEL_COUNT = sizeof(LEVELS) / sizeof(LevelType);

LevelType currentLevel;

static int updated;
static int level = 0;

static void DrawLevel(const LevelType *level);

/** Load the first level. */
void SetFirstLevel() {
   ResetScore();
   didWin = 0;
   didLose = 0;
   didDie = 0;
   level = -1;
   NextLevel();
}

/** Load the next level. */
void NextLevel() {
   int x;

   level = (level + 1) % LEVEL_COUNT;

   currentLevel.x = LEVELS[level].x;
   currentLevel.y = LEVELS[level].y;

   for(x = 0; x < LEVEL_WIDTH * LEVEL_HEIGHT; x++) {
      currentLevel.data[x] = LEVELS[level].data[x];
   }

   updated = 1;
   didWin = 0;

   ResetBonus();
   SetBallForLevel();

   Redraw();

}

/** Redraw the current level. */
void Redraw() {
   if(updated) {
      ClearScreen();
      DrawLevel(&currentLevel);
      DrawBall();
      updated = 0;
   }
   DrawRHS();
   XCopyArea(display, buffer, mainWindow, mainGC, 0, 0,
      WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
}

/** Mark the level as updated. */
void SetUpdated() {
   updated = 1;
}

/** Draw a specific block. */
void UpdateBlock(int x, int y) {
   DrawBlock(currentLevel.data[y * LEVEL_WIDTH + x], x, y);
}

/** Draw a level. */
void DrawLevel(const LevelType *level) {
   BlockType b;
   int x, y;

   for(y = 0; y < LEVEL_HEIGHT; y++) {
      for(x = 0; x < LEVEL_WIDTH; x++) {
         b = level->data[y * LEVEL_WIDTH + x];
         DrawBlock(b, x, y);
      }
   }
}

/** Get the current level. */
int GetLevel() {
   return level + 1;
}


