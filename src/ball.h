/**
 * @file ball.h
 * @author Joe Wingbermuehle
 */

#ifndef BALL_H
#define BALL_H

struct ImageType;

extern struct ImageType *ballImage;

/** Load the ball image. */
void LoadBall();

/** Unload the ball image. */
void UnloadBall();

/** Place the ball for starting a level. */
void SetBallForLevel();

/** Place the ball for restarting a level. */
void SetBallForRestart();

/** Draw the ball at its current location. */
void DrawBall();

/** Erase the ball. */
void EraseBall();

/** Move the ball in its current direction. */
void MoveBall();

/** Move the ball right one unit. */
void MoveBallRight();

/** Move the ball left one unit. */
void MoveBallLeft();

void DrawKeyBlock();
void DrawModeBlock();

#endif /* BALL_H */

