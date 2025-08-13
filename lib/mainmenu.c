#include "mainmenu.h"
#include "raylib.h"

static float buttonRoundness = 0.2f;
static float buttonWidth;  // = 200.0f;
static float buttonHeight; // = 100.0f;
static float buttonSegments = 0.0f;
static float buttonLineThick = 1.0f;

static Rectangle practiceButtonRec;
static Color practiceButtonColor;
static Color practiceButtonTextColor;

static Rectangle local2PButtonRec;
static Color local2PButtonColor;
static Color local2PButtonTextColor;

static Color buttonColors[2] = {GRAY, LIGHTGRAY};
static Color textColors[2] = {MAROON, RED};

Rectangle GetPracticeRec() { return practiceButtonRec; }

Rectangle GetLocal2PRec() { return local2PButtonRec; }

void InitMainMenu() {
  buttonWidth = (float)GetScreenWidth() / 3;
  buttonHeight = (float)GetScreenHeight() / 6;

  practiceButtonRec.x = ((float)GetScreenWidth() - buttonWidth) / 2;
  practiceButtonRec.y = ((float)GetScreenHeight() - buttonHeight) / 3;
  practiceButtonRec.width = buttonWidth;
  practiceButtonRec.height = buttonHeight;

  local2PButtonRec.x = ((float)GetScreenWidth() - buttonWidth) / 2;
  local2PButtonRec.y = ((float)GetScreenHeight() - buttonHeight) * 2 / 3;
  local2PButtonRec.width = buttonWidth;
  local2PButtonRec.height = buttonHeight;
}

void UpdateMainMenu() {
  Vector2 mousePos = GetMousePosition();

  bool practiceButtonSelected =
      CheckCollisionPointRec(mousePos, practiceButtonRec);
  practiceButtonColor = buttonColors[practiceButtonSelected];
  practiceButtonTextColor = textColors[practiceButtonSelected];

  bool twoPlayerButtonSelected =
      CheckCollisionPointRec(mousePos, local2PButtonRec);
  local2PButtonColor = buttonColors[twoPlayerButtonSelected];
  local2PButtonTextColor = textColors[twoPlayerButtonSelected];
}

void DrawMainMenu() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
  DrawText("Main Menu", 20, 20, 60, MAROON);

  DrawRectangleRounded(practiceButtonRec, buttonRoundness, buttonSegments,
                       practiceButtonColor);

  DrawText("Practice Mode", practiceButtonRec.x + 80,
           practiceButtonRec.y + (practiceButtonRec.height / 2) - 30, 60,
           practiceButtonTextColor);

  DrawRectangleRounded(local2PButtonRec, buttonRoundness, buttonSegments,
                       local2PButtonColor);
  DrawText("Two Player Mode", local2PButtonRec.x + 35,
           local2PButtonRec.y + (local2PButtonRec.height / 2) - 30, 60,
           local2PButtonTextColor);
}
