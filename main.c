#include "mainmenu.h"
#include "practice.h"
#include "raylib.h"
#include "start.h"

#define SCREENWIDTH 1800
#define SCREENHEIGHT 1440

typedef enum { START = 0, MAINMENU, PRACTICE } GameScene;

static GameScene currentScene = START;
static const int screenWidth = SCREENWIDTH;
static const int screenHeight = SCREENHEIGHT;
static float deltaTime = 0.0f;

void update() {
  deltaTime = GetFrameTime();
  switch (currentScene) {
  case START:
    UpdateStartScene();
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
      currentScene = MAINMENU;
    }
    break;
  case MAINMENU:
    UpdateMainMenu();
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
      currentScene = PRACTICE;
      InitPracticeScene();
    }
    break;
  case PRACTICE:
    UpdatePracticeScene(deltaTime);
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
