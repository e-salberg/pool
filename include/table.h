#pragma once
#include "ball.h"
#include "physics.h"

// should be 2:1 ratio tournament 9ft table is 100in to 50in?
#define TABLEWIDTH 1400
#define TABLEHEIGHT 700

typedef struct {
  float x;
  float y;
  float radius;
} Circle;

Rectangle getTable();
Vector2 ResetBallAfterScratch(Ball b);
void handleBallToWallCollision(Ball *b);
bool checkBallWentInPocket(Ball b);
void InitTable();
void DrawTable();
