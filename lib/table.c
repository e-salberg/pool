#include "table.h"
#include "raylib.h"

#define POCKET_RADIUS 35
#define TOTAL_POCKETS 6

// TODO - change this to make sense
static Rectangle table = {
    .x = SCREENWIDTH / 10.0f,
    .y = SCREENHEIGHT / 4.0f,
    .width = TABLEWIDTH,
    .height = TABLEHEIGHT,
};

static Circle pockets[TOTAL_POCKETS];

Rectangle getTable() { return table; }

void InitTable() {
  // Top Left
  pockets[0].x = table.x;
  pockets[0].y = table.y;

  // Top Right
  pockets[1].x = table.x + table.width;
  pockets[1].y = table.y;

  // Bot Left
  pockets[2].x = table.x;
  pockets[2].y = table.y + table.height;

  // Bot Right
  pockets[3].x = table.x + table.width;
  pockets[3].y = table.y + table.height;

  // Top Middle
  pockets[4].x = table.x + (table.width) / 2;
  pockets[4].y = table.y - (POCKET_RADIUS * .33);

  // Bot Middle
  pockets[5].x = table.x + (table.width) / 2;
  pockets[5].y = table.y + table.height + (POCKET_RADIUS * .33);

  for (int i = 0; i < TOTAL_POCKETS; i++) {
    pockets[i].radius = POCKET_RADIUS;
  }
}

bool checkBallWentInPocket(Ball b) {
  for (int i = 0; i < TOTAL_POCKETS; i++) {
    Vector2 p = {.x = pockets[i].x, .y = pockets[i].y};
    if (distance(b.position, p) <= (b.radius + pockets[i].radius) * .75) {
      return true;
    }
  }
  return false;
}

void handleBallToWallCollision(Ball *b) {
  if (b->position.x >= table.x + table.width - b->radius) {
    b->position.x = table.x + table.width - b->radius;
    b->velocity.x *= -RESTITUTION_BALL_WALL;
  } else if (b->position.x <= table.x + b->radius) {
    b->position.x = table.x + b->radius;
    b->velocity.x *= -RESTITUTION_BALL_WALL;
  }

  if (b->position.y >= table.y + table.height - b->radius) {
    b->position.y = table.y + table.height - b->radius;
    b->velocity.y *= -RESTITUTION_BALL_WALL;
  } else if (b->position.y <= table.y + b->radius) {
    b->position.y = table.y + b->radius;
    b->velocity.y *= -RESTITUTION_BALL_WALL;
  }
}

void DrawTable() {
  // TODO - make this nicer than just random numbers
  DrawRectangle(table.x - 50, table.y - 50, table.width + 100,
                table.height + 100, BEIGE); // table boarder
  DrawRectangle(table.x - 10, table.y - 10, table.width + 20, table.height + 20,
                LIME); // visual walls
  DrawRectangle(table.x, table.y, table.width, table.height,
                DARKGREEN); // play area

  for (int i = 0; i < TOTAL_POCKETS; i++) {
    DrawCircle(pockets[i].x, pockets[i].y, pockets[i].radius, BLACK);
  }
}
