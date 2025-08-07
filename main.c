#include "physics.h"
#include "raylib.h"

static const int screenWidth = 1200;
static const int screenHeight = 960;

static float deltaTime = 0.0f;
static bool hasqBallBeenHit = false;
static Vector2 mousePosition = {0};

static Ball qBall = {
    .position = {(float)screenWidth / 2, (float)screenHeight / 2},
    .velocity = {0.0f, 0.0f},
    .mass = 10,
    .radius = 50};
static Ball redBall = {
    .position = {(float)screenWidth / 2, (float)screenHeight / 4},
    .velocity = {0.0f, 0.0f},
    .mass = 10,
    .radius = 50};

void update() {
  deltaTime = GetFrameTime();

  if (!hasqBallBeenHit) {
    mousePosition = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      hasqBallBeenHit = true;
      qBall.velocity.x = 3 * (qBall.position.x - mousePosition.x);
      qBall.velocity.y = 3 * (qBall.position.y - mousePosition.y);
    }
  }

  qBall.position.x += qBall.velocity.x * deltaTime;
  qBall.position.y += qBall.velocity.y * deltaTime;
  redBall.position.x += redBall.velocity.x * deltaTime;
  redBall.position.y += redBall.velocity.y * deltaTime;

  if (hasCollided(qBall, redBall)) {
    handleBallToBallCollision(&qBall, &redBall);
  }
  handleBallToWallCollision(&qBall);
  handleBallToWallCollision(&redBall);
}

void draw() {
  BeginDrawing();
  ClearBackground(DARKGREEN);
  DrawText(TextFormat("Q - x:%.0f, y:%.0f", qBall.velocity.x, qBall.velocity.y),
           10, 30, 20, WHITE);
  DrawText(TextFormat("RED - x:%.0f, y:%.0f", redBall.velocity.x,
                      redBall.velocity.y),
           10, 10, 20, RED);

  DrawCircleV(qBall.position, qBall.radius, WHITE);
  DrawCircleV(redBall.position, redBall.radius, RED);
  EndDrawing();
}

int main(void) {

  InitWindow(screenWidth, screenHeight, "raylib window");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    update();
    draw();
  }

  return 0;
}
