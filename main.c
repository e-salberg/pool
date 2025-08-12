#include "local2p.h"
#include "mainmenu.h"
#include "practice.h"
#include "raylib.h"
#include "start.h"

#define SCREENWIDTH 1800
#define SCREENHEIGHT 1440

typedef enum { START = 0, MAINMENU, PRACTICE, LOCAL2P } GameScene;

static GameScene currentScene = START;
static const int screenWidth = SCREENWIDTH;
static const int screenHeight = SCREENHEIGHT;
static float deltaTime = 0.0f;
static Vector2 mousePos = {0};

void update() {
  deltaTime = GetFrameTime();
  mousePos = GetMousePosition();
  switch (currentScene) {
  case START:
    UpdateStartScene();
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
      currentScene = MAINMENU;
      InitMainMenu();
    }
    break;
  case MAINMENU:
    UpdateMainMenu();
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
      if (CheckCollisionPointRec(mousePos, GetPracticeRec())) {
        currentScene = PRACTICE;
        InitPracticeScene();
      } else if (CheckCollisionPointRec(mousePos, GetLocal2PRec())) {
        currentScene = LOCAL2P;
        InitLocal2pScene();
      }
    }
    break;
  case PRACTICE:
    if (IsKeyPressed(KEY_R)) {
      InitPracticeScene();
    }
    UpdatePracticeScene(deltaTime);
    break;
  case LOCAL2P:
    UpdateLocal2pScene(deltaTime);
    break;
  }
}

void draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  switch (currentScene) {
  case START:
    DrawStartScene();
    break;
  case MAINMENU:
    DrawMainMenu();
    break;
  case PRACTICE:
    DrawPracticeScene();
    break;
  case LOCAL2P:
    DrawLocal2pScene();
    break;
  }
  EndDrawing();
}

int main(void) {

  InitWindow(screenWidth, screenHeight, "raylib window");
  SetTargetFPS(120);

  while (!WindowShouldClose()) {
    update();
    draw();
  }
  CloseWindow();

  return 0;
}
