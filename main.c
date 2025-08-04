#include "physics.h"
#include "raylib.h"

int main(void) {
  const int screenWidth = 1200;
  const int screenHeight = 960;
  float deltaTime = 0.0f;
  bool hasBallBeenHit = false;
  Vector2 mousePosition = {0};

  Ball ball = {.position = {(float)screenWidth / 2, (float)screenHeight / 2},
               .velocity = {0.0f, 0.0f},
               .mass = 10,
               .radius = 50};

  InitWindow(screenWidth, screenHeight, "raylib window");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    deltaTime = GetFrameTime();

    if (!hasBallBeenHit) {
      mousePosition = GetMousePosition();
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        hasBallBeenHit = true;
        ball.velocity.x = ball.position.x - mousePosition.x;
        ball.velocity.y = ball.position.y - mousePosition.y;
      }
    }

    ball.position.x += ball.velocity.x * deltaTime;
    ball.position.y += ball.velocity.y * deltaTime;
    if (ball.position.x >= (GetScreenWidth() - ball.radius) ||
        ball.position.x <= ball.radius) {
      ball.velocity.x *= -1.0f;
    }
    if (ball.position.y >= (GetScreenHeight() - ball.radius) ||
        ball.position.y <= ball.radius) {
      ball.velocity.y *= -1.0f;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (!hasBallBeenHit) {
      DrawText(TextFormat("Mouse Position - x:%.0f, y:%.0f", mousePosition.x,
                          mousePosition.y),
               10, 10, 20, LIGHTGRAY);
    }
    DrawCircleV(ball.position, ball.radius, MAROON);
    EndDrawing();
  }

  return 0;
}
