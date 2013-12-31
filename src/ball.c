/**
 * @file ball.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "ball.h"
#include "image.h"
#include "level.h"
#include "main.h"
#include "score.h"
#include "block.h"
#include "background.h"

#include "images/ball.xpm"

/** Ball directions. */
typedef enum {
   DIR_DOWN,
   DIR_UP
} DirectionType;


static const int BONUS_DELAY = 60;

static const int BALL_SIZE = 16;

static const int MODE_X = BLOCK_WIDTH * LEVEL_WIDTH
   + RHS_WIDTH / 2 - BLOCK_WIDTH / 2;
static const int MODE_Y = 267;
static const int KEY_X = BLOCK_WIDTH * LEVEL_WIDTH
   + RHS_WIDTH / 2 - BLOCK_WIDTH / 2;
static const int KEY_Y = 193;

static int blockCount;
static int endBlockCount;

static int ballx, bally;
static DirectionType balld;
static int mode;
static int haveKey;
static int isReversed;

ImageType *ballImage = NULL;

static void RemoveBlock(char *b);
static int CheckCollision(int x, int y);
static int CheckGrid(int x, int y);
static void DoCollision(char *b);
static void CheckMode();
static void ComputeBlockCount();

/** Load the ball image. */
void LoadBall() {
   ballImage = LoadImage(ball_xpm);
}

/** Unload the ball image. */
void UnloadBall() {
   DestroyImage(ballImage);
}

/** Place the ball for starting a level. */
void SetBallForLevel() {
   haveKey = 0;
   ComputeBlockCount();
   SetBallForRestart();
}

/** Place the ball for restarting a level. */
void SetBallForRestart() {
   ballx = currentLevel.x * BLOCK_WIDTH + BLOCK_WIDTH / 2 - BALL_SIZE / 2;
   bally = currentLevel.y * BLOCK_HEIGHT + BLOCK_HEIGHT / 2 - BALL_SIZE / 2;
   balld = DIR_DOWN;
   mode = BLOCK_BEGIN;
   CheckMode();
}

/** Draw the ball. */
void DrawBall() {
   DrawImage(buffer, bufferGC, ballImage, ballx, bally);
}

/** Erase the ball. */
void EraseBall() {
   ClearArea(ballx, bally, BALL_SIZE, BALL_SIZE);
}

/** Move the ball in its current direction. */
void MoveBall() {

   static int moveCount = 0;
   int ny;

   if(balld == DIR_UP) {
      ny = bally - BALL_DELTA_Y;
   } else {
      ny = bally + BALL_DELTA_Y;
   }

   if(!CheckCollision(ballx, ny)) {
      bally = ny;
   } else {
      if(balld == DIR_UP) {
         balld = DIR_DOWN;
      } else {
         balld = DIR_UP;
      }
   }

   moveCount = (moveCount + 1) % BONUS_DELAY;
   if(!moveCount) {
      UpdateBonus(-1);
   }

}

/** Move the ball left one unit. */
void MoveBallLeft() {
   int nx = ballx;

   if(isReversed) {
       nx += BALL_DELTA_X;
   } else {
       nx -= BALL_DELTA_X;
   }
   if(!CheckCollision(nx, bally)) {
      ballx = nx;
   }
}

/** Move the ball right one unit. */
void MoveBallRight() {
   int nx = ballx;

   if(isReversed) {
      nx -= BALL_DELTA_X;
   } else {
      nx += BALL_DELTA_X;
   }
   if(!CheckCollision(nx, bally)) {
      ballx = nx;
   }
}

/** Check for a collision. */
int CheckCollision(int x, int y) {

   int value;

   if(x < 0 || x >= BLOCK_WIDTH * LEVEL_WIDTH - BALL_SIZE) {
      return 1;
   }
   if(y < 0 || y >= BLOCK_HEIGHT * LEVEL_HEIGHT - BALL_SIZE) {
      return 1;
   }

   value = CheckGrid(x, y);
   if(didDie) {
      return 1;
   }
   value |= CheckGrid(x + BALL_SIZE, y);
   if(didDie) {
      return 1;
   }
   value |= CheckGrid(x, y + BALL_SIZE);
   if(didDie) {
      return 1;
   }
   value |= CheckGrid(x + BALL_SIZE, y + BALL_SIZE);
   if(didDie) {
      return 1;
   }

   return value;

}

/****************************************************************************
 ****************************************************************************/
int CheckGrid(int x, int y) {
   char *b;

   x /= BLOCK_WIDTH;
   y /= BLOCK_HEIGHT;

   if(x > LEVEL_WIDTH || y > LEVEL_HEIGHT) {
      return 1;
   }

   b = &currentLevel.data[y * LEVEL_WIDTH + x];
   if(*b) {
      DoCollision(b);
      UpdateBlock(x, y);
      return 1;
   } else {
      return 0;
   }
}

/****************************************************************************
 ****************************************************************************/
void DoCollision(char *b) {
   BlockType lastMode;

   lastMode = mode;

   if(mode == *b) {
      RemoveBlock(b);
   }

   switch(*b) {
   case BLOCK_KILLER:
      UpdateLives(-1);
      SetBallForRestart();
      didDie = 1;
      break;
   case BLOCK_KEY:
      if(!haveKey) {
         haveKey = 1;
         RemoveBlock(b);
      }
      break;
   case BLOCK_LOCK:
      if(haveKey) {
         haveKey = 0;
         RemoveBlock(b);
      }
      break;
   case BLOCK_SWAP:
      RemoveBlock(b);
      isReversed = (isReversed + 1) % 2;
      break;
   default:
      break;
   }

   if(mode != BLOCK_END) {
      switch(*b) {
      case BLOCK_RED_CHANGE:
         mode = BLOCK_RED;
         break;
      case BLOCK_GREEN_CHANGE:
         mode = BLOCK_GREEN;
         break;
      case BLOCK_BLUE_CHANGE:
         mode = BLOCK_BLUE;
         break;
      case BLOCK_YELLOW_CHANGE:
         mode = BLOCK_YELLOW;
         break;
      default:
         break;
      }
   }

   if(mode != lastMode) {
      DrawModeBlock();
   }
}

/****************************************************************************
 ****************************************************************************/
void RemoveBlock(char *b) {
   if(*b == BLOCK_END) {
      --endBlockCount;
   } else {
      --blockCount;
   }
   *b = BLOCK_NONE;
   UpdateScore(5);
   CheckMode();
}

/****************************************************************************
 ****************************************************************************/
void CheckMode() {
   if(!blockCount) {
      mode = BLOCK_END;
   }
   if(!endBlockCount) {
      didWin = 1;
   }
}

/****************************************************************************
 ****************************************************************************/
void ComputeBlockCount() {
   int x;

   blockCount = 0;
   endBlockCount = 0;

   for(x = 0; x < LEVEL_WIDTH * LEVEL_HEIGHT; x++) {
      switch(currentLevel.data[x]) {
      case BLOCK_BEGIN:
      case BLOCK_RED:
      case BLOCK_GREEN:
      case BLOCK_BLUE:
      case BLOCK_YELLOW:
      case BLOCK_LOCK:
      case BLOCK_KEY:
      case BLOCK_SWAP:
         ++blockCount;
         break;
      case BLOCK_END:
         ++endBlockCount;
         break;
      default:
         break;
      }
   }

}

/****************************************************************************
 ****************************************************************************/
void DrawKeyBlock() {
   if(haveKey) {
      DrawBlockExact(BLOCK_KEY, KEY_X, KEY_Y);
   } else {
      DrawBlockExact(BLOCK_LOCK, KEY_X, KEY_Y);
   }
}

/****************************************************************************
 ****************************************************************************/
void DrawModeBlock() {
   DrawBlockExact(mode, MODE_X, MODE_Y);
}

