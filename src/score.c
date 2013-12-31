/**
 * @file score.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "score.h"
#include "main.h"
#include "image.h"
#include "block.h"
#include "ball.h"
#include "level.h"
#include "font.h"
#include "background.h"

#include "images/rhs.xpm"
#include "images/bonus.xpm"
#include "images/paused.xpm"
#include "images/game-over.xpm"

static const int LIFE_X = BLOCK_WIDTH * LEVEL_WIDTH + 6;
static const int LIFE_Y = 73;
static const int LEVEL_X = BLOCK_WIDTH * LEVEL_WIDTH + 36;
static const int LEVEL_Y = 18;
static const int SCORE_X = BLOCK_WIDTH * LEVEL_WIDTH + 36;
static const int SCORE_Y = 39;
static const int BONUS_X = BLOCK_WIDTH * LEVEL_WIDTH + 36;
static const int BONUS_Y = 60;

static ImageType *scoreImage = NULL;
static ImageType *bonusImage = NULL;
static ImageType *pausedImage = NULL;
static ImageType *gameOverImage = NULL;

static int updated = 0;

static int score;
static int bonus;
static int lives;

/** Load the score board data. */
void LoadScore() {

   scoreImage = LoadImage(rhs_xpm);
   bonusImage = LoadImage(bonus_xpm);
   pausedImage = LoadImage(paused_xpm);
   gameOverImage = LoadImage(game_over_xpm);

   ResetScore();

}

/** Reset the score. */
void ResetScore() {
   updated = 1;
   score = 0;
   lives = 5;
}

/** Unload the score data. */
void UnloadScore() {

   DestroyImage(scoreImage);
   DestroyImage(bonusImage);
   DestroyImage(pausedImage);
   DestroyImage(gameOverImage);

}

/** Update the score. */
void UpdateScore(int delta) {
   if((score % 1000) + delta >= 1000) {
      UpdateLives(1);
   }
   score += delta;
   updated = 1;
}

/** Update the number of lives left. */
void UpdateLives(int delta) {
   lives += delta;
   if(lives <= 0) {
      lives = 0;
      didLose = 1;
      if(gameOverImage) {
         DrawImage(buffer, bufferGC, gameOverImage, 
            BLOCK_WIDTH * LEVEL_WIDTH / 2 - gameOverImage->image->width / 2,
            BLOCK_HEIGHT * LEVEL_HEIGHT / 2 - gameOverImage->image->height / 2);
      }
   } else if(lives > 5) {
      lives = 5;
   }
   updated = 1;
}

/** Update the bonus counter. */
void UpdateBonus(int delta) {
   bonus += delta;
   if(bonus < 0) {
      bonus = 0;
   }
   updated = 1;
}

/** Set that the RHS should be redrawn. */
void SetRHSUpdated() {
   updated = 1;
}

/** Draw the score board. */
void DrawRHS() {

   if(updated) {
      DrawImage(buffer, bufferGC, scoreImage, BLOCK_WIDTH * LEVEL_WIDTH, 0);
      DrawLevelNumber();
      DrawScore();
      DrawBonus();
      DrawLives();
      DrawKeyBlock();
      DrawModeBlock();
      updated = 0;
   }

}

/** Draw the level number. */
void DrawLevelNumber() {
   static char str[10];
   snprintf(str, 10, "%8d", GetLevel());
   XSetForeground(display, bufferGC, foreground);
   XSetFont(display, bufferGC, GetScoreFont()->fid);
   XDrawString(display, buffer, bufferGC, LEVEL_X, LEVEL_Y,
      str, strlen(str));
}

/** Draw the score. */
void DrawScore() {
   static char str[10];
   snprintf(str, 10, "%8d", score);
   XSetForeground(display, bufferGC, foreground);
   XSetFont(display, bufferGC, GetScoreFont()->fid);
   XDrawString(display, buffer, bufferGC, SCORE_X, SCORE_Y,
      str, strlen(str));
}

/** Draw the bonus counter. */
void DrawBonus() {
   static char str[10];
   snprintf(str, 10, "%8d", bonus);
   XSetForeground(display, bufferGC, foreground);
   XSetFont(display, bufferGC, GetScoreFont()->fid);
   XDrawString(display, buffer, bufferGC, BONUS_X, BONUS_Y,
      str, strlen(str));
}

/** Draw the number of lives left. */
void DrawLives() {
   int x;

   if(ballImage) {
      for(x = 0; x < lives; x++) {
         DrawImage(buffer, bufferGC, ballImage, 24 * x + LIFE_X, LIFE_Y);
      }
   }
}

/** Reset the bonus counter. */
void ResetBonus() {
   bonus = 60;
   updated = 1;
}

/** Get the current bonus count. */
int GetBonus() {
   return bonus;
}

/** Pause/unpause the game. */
void SetPaused(int p) {
   if(p) {
      DrawImage(buffer, bufferGC, pausedImage, 
         BLOCK_WIDTH * LEVEL_WIDTH / 2 - pausedImage->image->width / 2,
         BLOCK_HEIGHT * LEVEL_HEIGHT / 2 - pausedImage->image->height / 2);
   } else {
      SetUpdated();
   }
   Redraw();
}

/** Update the bonus counter. */
void UpdateBonusCountdown() {
   static int state = 0;

   if(bonus) {
      --bonus;
      ++score;
      updated = 1;
   }

   state = (state + 1) % 2;

   if(!state) {
      DrawImage(buffer, bufferGC, bonusImage, 
         BLOCK_WIDTH * LEVEL_WIDTH / 2 - bonusImage->image->width / 2,
         BLOCK_HEIGHT * LEVEL_HEIGHT / 2 - bonusImage->image->height / 2);
   } else {
      ClearArea(BLOCK_WIDTH * LEVEL_WIDTH / 2 - bonusImage->image->width / 2,
         BLOCK_HEIGHT * LEVEL_HEIGHT / 2 - bonusImage->image->height / 2,
         bonusImage->image->width, bonusImage->image->height);
   }

   Redraw();

}

