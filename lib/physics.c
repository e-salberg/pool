#include "physics.h"
#include <math.h>

float sqr(float x) { return x * x; }

float distance(Vector2 a, Vector2 b) {
  return sqrtf(sqr(a.x - b.x) + sqr(a.y - b.y));
}

float magnitude(Vector2 vec) {
  return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

void normalize(Vector2 *vec) {
  float mag = magnitude(*vec);
  if (mag) {
    vec->x = vec->x / mag;
    vec->y = vec->y / mag;
  }
}

float dot_product(Vector2 a, Vector2 b) { return (a.x * b.x) + (a.y * b.y); }

bool hasCollided(Ball b1, Ball b2) {
  return distance(b1.position, b2.position) <= b1.radius + b2.radius;
}

void handleBallToBallCollision(Ball *b1, Ball *b2) {
  Vector2 direction = {.x = b2->position.x - b1->position.x,
                       .y = b2->position.y - b1->position.y};

  // since balls are overlapping, need to move them away to 'just be touching'
  float correction = (b1->radius + b2->radius - magnitude(direction)) / 2;
  normalize(&direction);
  b1->position.x += (direction.x * -correction);
  b1->position.y += (direction.y * -correction);
  b2->position.x += (direction.x * correction);
  b2->position.y += (direction.y * correction);

  float v1 = dot_product(b1->velocity, direction);
  float v2 = dot_product(b2->velocity, direction);

  float v1final = (b1->mass * v1 + b2->mass * v2 -
                   b2->mass * (v1 - v2) * RESTITUTION_BALL_BALL) /
                  (b1->mass + b2->mass);
  float v2final = (b1->mass * v1 + b2->mass * v2 -
                   b1->mass * (v2 - v1) * RESTITUTION_BALL_BALL) /
                  (b1->mass + b2->mass);

  b1->velocity.x += (direction.x * (v1final - v1));
  b1->velocity.y += (direction.y * (v1final - v1));
  b2->velocity.x += (direction.x * (v2final - v2));
  b2->velocity.y += (direction.y * (v2final - v2));
}

void handleBallToWallCollision(Ball *b) {
  if (b->position.x >= (GetScreenWidth() - b->radius) ||
      b->position.x <= b->radius) {
    b->velocity.x *= (-1.0f * RESTITUTION_BALL_WALL);
  }
  if (b->position.y >= (GetScreenHeight() - b->radius) ||
      b->position.y <= b->radius) {
    b->velocity.y *= (-1.0f * RESTITUTION_BALL_WALL);
  }
}
