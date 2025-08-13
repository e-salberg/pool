#pragma once

#include "ball.h"

#define MAX_BALLS 16
#define TOTAL_PLAYER_BALLS ((MAX_BALLS - 2) / 2)
#define BALL_MASS 10
#define BALL_RADIUS 25

typedef struct {
  int balls[TOTAL_PLAYER_BALLS];
  int score;
  BALL_TYPE assignedBallType;
} PlayerArray;

void InitLocal2pScene();
void UpdateLocal2pScene(float deltaTime);
void DrawLocal2pScene();
