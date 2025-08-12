#pragma once

#include "raylib.h"

#define RESTITUTION_BALL_BALL .9f
#define RESTITUTION_BALL_WALL .5f
#define ROLLING_RESISTANCE 5.0f // if m/s this should be .01?
#define GRAVITY 9.8f
#define ERROR 5

typedef struct {
  Vector2 position;
  Vector2 velocity;
  float mass;
  int radius;
} Ball;

typedef enum { NONE, Q, SOLID, STRIPE, EIGHT } BALL_TYPE;

void UpdateBallPosition(Ball *b, float deltaTime);
void UpdateBallVelocity(Ball *b, float deltaTime);
void DrawBall(Ball b, BALL_TYPE type, Color color);
bool hasCollided(Ball b1, Ball b2);
void handleBallToBallCollision(Ball *b1, Ball *b2);
