#pragma one

#include <raylib.h>
#include <stdbool.h>

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

bool hasCollided(Ball b1, Ball b2);
void handleBallToBallCollision(Ball *b1, Ball *b2);
void handleBallToWallCollision(Ball *b);
void accelerateBall(Ball *b, float deltaTime);
