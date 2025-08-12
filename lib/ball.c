#include "ball.h"
#include "math.h"
#include "physics.h"
#include "raylib.h"

// TODO - maybe?
// not sure if updating position and velocity should happen at the same time or
// update position -> check for collisions -> update velocity
void UpdateBallPosition(Ball *b, float deltaTime) {
  // Update position
  b->position.x += b->velocity.x * deltaTime;
  b->position.y += b->velocity.y * deltaTime;
}
void DrawBall(Ball b, BALL_TYPE type, Color color) {
  switch (type) {
  case Q:
  case EIGHT:
  case SOLID:
    DrawCircleV(b.position, b.radius, color);
    break;
  case STRIPE:
    DrawCircleV(b.position, b.radius, WHITE);
    float startAngle = (float)(-360.0f / 8);
    float endAngle = (float)(360.0f / 8);
    DrawCircleSector(b.position, b.radius, startAngle, endAngle, 0, color);
    DrawCircleSector(b.position, b.radius, startAngle + 180, endAngle + 180, 0,
                     color);
    Vector2 recSize = {b.radius * 1.4f, b.radius * 1.4f};
    Vector2 recPos = {b.position.x - (recSize.x / 2),
                      b.position.y - (recSize.y / 2)};
    DrawRectangleV(recPos, recSize, color);
    break;
  case NONE:
    break;
  }
}

bool hasCollided(Ball b1, Ball b2) {
  return distance(b1.position, b2.position) <= b1.radius + b2.radius;
}

void handleBallToBallCollision(Ball *b1, Ball *b2) {
  Vector2 direction = {.x = b2->position.x - b1->position.x,
                       .y = b2->position.y - b1->position.y};

  // since balls are overlapping, need to move them away to 'just be touching'
  float correction = (b1->radius + b2->radius - magnitude(direction)) / 2;
  Vector2 normalizedDir = normalize(direction);
  b1->position.x += (normalizedDir.x * -correction);
  b1->position.y += (normalizedDir.y * -correction);
  b2->position.x += (normalizedDir.x * correction);
  b2->position.y += (normalizedDir.y * correction);

  float v1 = dot_product(b1->velocity, normalizedDir);
  float v2 = dot_product(b2->velocity, normalizedDir);

  float v1final = (b1->mass * v1 + b2->mass * v2 -
                   b2->mass * (v1 - v2) * RESTITUTION_BALL_BALL) /
                  (b1->mass + b2->mass);
  float v2final = (b1->mass * v1 + b2->mass * v2 -
                   b1->mass * (v2 - v1) * RESTITUTION_BALL_BALL) /
                  (b1->mass + b2->mass);

  b1->velocity.x += (normalizedDir.x * (v1final - v1));
  b1->velocity.y += (normalizedDir.y * (v1final - v1));
  b2->velocity.x += (normalizedDir.x * (v2final - v2));
  b2->velocity.y += (normalizedDir.y * (v2final - v2));
}

void UpdateBallVelocity(Ball *b, float deltaTime) {
  float a = ROLLING_RESISTANCE * GRAVITY * deltaTime;
  Vector2 direction = normalize(b->velocity);

  float ax = direction.x * a;
  if (fabs(b->velocity.x) <= fabs(a)) {
    b->velocity.x = 0;
  } else {
    b->velocity.x -= ax;
  }
  float ay = direction.y * a;
  if (fabs(b->velocity.y) <= fabs(a)) {
    b->velocity.y = 0;
  } else {
    b->velocity.y -= ay;
  }
}
