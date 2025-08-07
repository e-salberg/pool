#pragma one

#include <raylib.h>
#include <stdbool.h>

#define RESTITUTION_BALL_BALL 1
#define RESTITUTION_BALL_WALL 1

typedef struct {
  Vector2 position;
  Vector2 velocity;
  float mass;
  int radius;
} Ball;

bool hasCollided(Ball b1, Ball b2);
void handleBallToBallCollision(Ball *b1, Ball *b2);
void handleBallToWallCollision(Ball *b);
