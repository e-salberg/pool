#include "table.h"
#include "raylib.h"

#define POCKET_RADIUS 35
#define TOTAL_POCKETS 6

static Rectangle table;

static Circle pockets[TOTAL_POCKETS];

void InitTable() {
  // TODO - change this to make sense
  table.x = GetScreenWidth() / 10.0f;
  table.y = GetScreenHeight() / 4.0f;
  table.width = TABLEWIDTH;
  table.height = TABLEHEIGHT;

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

Rectangle getTable() { return table; }

Vector2 ResetBallAfterScratch(Ball b) {
  Vector2 mousePos = GetMousePosition();
  Rectangle resetBox = {
      .x = table.x,
      .y = table.y,
      .width = table.width / 3,
      .height = table.height,
  };

  Vector2 newPos = mousePos;
  if (mousePos.x > resetBox.x + resetBox.width - b.radius) {
    newPos.x = resetBox.x + resetBox.width - b.radius;
  } else if (mousePos.x < resetBox.x + b.radius) {
    newPos.x = resetBox.x + b.radius;
  }
  if (mousePos.y > resetBox.y + resetBox.height - b.radius) {
    newPos.y = resetBox.y + resetBox.height - b.radius;
  } else if (mousePos.y < resetBox.y + b.radius) {
    newPos.y = resetBox.y + b.radius;
  }

  return newPos;
}
