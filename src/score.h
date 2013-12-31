/**
 * @file score.h
 * @author Joe Wingbermuehle
 */

#ifndef SCORE_H
#define SCORE_H

void LoadScore();
void UnloadScore();

void UpdateScore(int delta);
void UpdateLives(int delta);
void UpdateBonus(int delta);

void ResetScore();

void DrawRHS();

void DrawLevelNumber();
void DrawScore();
void DrawBonus();
void DrawLives();

void SetPaused(int p);

void ResetBonus();
int GetBonus();
void UpdateBonusCountdown();

#endif /* SCORE_H */

