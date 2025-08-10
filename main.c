#include "raylib.h"
#include "table.h"

#define MAX_BALLS 16
#define BALL_MASS 10
#define BALL_RADIUS 25

static const int screenWidth = SCREENWIDTH;
static const int screenHeight = SCREENHEIGHT;

typedef enum { Q, SOLID, STRIPE, EIGHT } BALL_TYPE;

static Ball physics[MAX_BALLS] = {0};
static bool onTable[MAX_BALLS];
static BALL_TYPE types[MAX_BALLS] = {
    Q,     SOLID,  SOLID,  SOLID,  SOLID,  SOLID,  SOLID,  SOLID,
    EIGHT, STRIPE, STRIPE, STRIPE, STRIPE, STRIPE, STRIPE, STRIPE};
static Color colors[MAX_BALLS] = {
    WHITE, YELLOW, BLUE, RED, PURPLE, ORANGE, GREEN, DARKBROWN,
    BLACK, YELLOW, BLUE, RED, PURPLE, ORANGE, GREEN, DARKBROWN};
static int startingOrder[MAX_BALLS] = {0, 1,  11, 5, 2,  8,  10, 9,
                                       4, 14, 7,  6, 15, 13, 3,  12};

static float deltaTime = 0.0f;
static bool canShoot = true;
static bool hasScratched = false;
static bool canPlaceQ = true;
static Vector2 mousePosition = {0};

void update() {
  deltaTime = GetFrameTime();
  mousePosition = GetMousePosition();

  if (hasScratched && canShoot) {
    physics[0].position.x = mousePosition.x;
    physics[0].position.y = mousePosition.y;

    canPlaceQ = true;
    for (int i = 1; i < MAX_BALLS; i++) {
      if (hasCollided(physics[0], physics[i])) {
        canPlaceQ = false;
      }
    }
    if (canPlaceQ && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      hasScratched = false;
    }
    return;
  }

  if (canShoot && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    canShoot = false;
    physics[0].velocity.x = 3 * (physics[0].position.x - mousePosition.x);
    physics[0].velocity.y = 3 * (physics[0].position.y - mousePosition.y);
  }

  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    }
    physics[i].position.x += physics[i].velocity.x * deltaTime;
    physics[i].position.y += physics[i].velocity.y * deltaTime;
  }

  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    }
    for (int j = i + 1; j < MAX_BALLS; j++) {
      if (!onTable[j]) {
        continue;
      }
      if (hasCollided(physics[i], physics[j])) {
        handleBallToBallCollision(&physics[i], &physics[j]);
      }
    }
  }

  bool allBallsStoppedMoving = true;
  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    } else if (checkBallWentInPocket(physics[i])) {
      onTable[i] = false;
      hasScratched = types[i] == Q;
      continue;
    }

    handleBallToWallCollision(&physics[i]);
    if (physics[i].velocity.x != 0 || physics[i].velocity.y != 0) {
      allBallsStoppedMoving = false;
      accelerateBall(&physics[i], deltaTime);
    }
  }

  if (allBallsStoppedMoving) {
    canShoot = true;
    onTable[0] = true;
  }
}

void draw() {
  BeginDrawing();
  ClearBackground(WHITE);
  DrawTable();
  DrawText(TextFormat("Q - x:%.0f, y:%.0f", physics[0].velocity.x,
                      physics[0].velocity.y),
           10, 30, 20, BLACK);

  if (canShoot) {
    DrawText(TextFormat("PLAYER TURN"), 200, 40, 60, BLACK);
  }
  if (hasScratched && !canPlaceQ) {
    DrawText(TextFormat("CAN'T PLACE Q HERE"), 200, 100, 60, RED);
  }

  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    }

    switch (types[i]) {
    case Q:
    case EIGHT:
    case SOLID:
      DrawCircleV(physics[i].position, physics[i].radius, colors[i]);
      break;
    case STRIPE:
      DrawCircleV(physics[i].position, physics[i].radius, WHITE);
      float startAngle = (float)(-360.0f / 8);
      float endAngle = (float)(360.0f / 8);
      DrawCircleSector(physics[i].position, physics[i].radius, startAngle,
                       endAngle, 0, colors[i]);
      DrawCircleSector(physics[i].position, physics[i].radius, startAngle + 180,
                       endAngle + 180, 0, colors[i]);
      Vector2 recSize = {physics[i].radius * 1.4f, physics[i].radius * 1.4f};
      Vector2 recPos = {physics[i].position.x - (recSize.x / 2),
                        physics[i].position.y - (recSize.y / 2)};
      DrawRectangleV(recPos, recSize, colors[i]);
    }
    DrawFPS(10, 10);
  }
  EndDrawing();
}

int main(void) {

  InitWindow(screenWidth, screenHeight, "raylib window");
  SetTargetFPS(60);
  InitTable();

  for (int i = 0; i < MAX_BALLS; i++) {
    physics[i].velocity.x = 0.0f;
    physics[i].velocity.y = 0.0f;
    physics[i].mass = BALL_MASS;
    physics[i].radius = BALL_RADIUS;
    onTable[i] = true;
  }

  int COLS = 5;
  int idx = 1;
  for (int col = 0; col < COLS; col++) {
    float baseHeight = ((float)screenHeight / 2) - (col * BALL_RADIUS);
    for (int i = 0; i <= col; i++) {
      int ball = startingOrder[idx];
      physics[ball].position.x =
          ((float)screenWidth * .6f) + (col * 1.75 * BALL_RADIUS);
      physics[ball].position.y = baseHeight + (i * 2 * BALL_RADIUS);
      idx++;
    }
  }
  physics[0].position.x = (float)screenWidth / 4;
  physics[0].position.y = (float)screenHeight / 2;

  while (!WindowShouldClose()) {
    update();
    draw();
  }

  return 0;
}
