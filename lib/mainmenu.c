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

static Rectangle twoPlayerButtonRec;
static Color twoPlayerButtonColor;
static Color twoPlayerButtonTextColor;

static Color buttonColors[2] = {GRAY, LIGHTGRAY};
static Color textColors[2] = {MAROON, RED};

Rectangle GetPracticeRec() { return practiceButtonRec; }

void InitMainMenu() {
  buttonWidth = (float)GetScreenWidth() / 3;
  buttonHeight = (float)GetScreenHeight() / 6;

  practiceButtonRec.x = ((float)GetScreenWidth() - buttonWidth) / 2;
  practiceButtonRec.y = ((float)GetScreenHeight() - buttonHeight) / 3;
  practiceButtonRec.width = buttonWidth;
  practiceButtonRec.height = buttonHeight;

  twoPlayerButtonRec.x = ((float)GetScreenWidth() - buttonWidth) / 2;
  twoPlayerButtonRec.y = ((float)GetScreenHeight() - buttonHeight) * 2 / 3;
  twoPlayerButtonRec.width = buttonWidth;
  twoPlayerButtonRec.height = buttonHeight;
}

void UpdateMainMenu() {
  Vector2 mousePos = GetMousePosition();

  bool practiceButtonSelected =
      CheckCollisionPointRec(mousePos, practiceButtonRec);
  practiceButtonColor = buttonColors[practiceButtonSelected];
  practiceButtonTextColor = textColors[practiceButtonSelected];

  bool twoPlayerButtonSelected =
      CheckCollisionPointRec(mousePos, twoPlayerButtonRec);
  twoPlayerButtonColor = buttonColors[twoPlayerButtonSelected];
  twoPlayerButtonTextColor = textColors[twoPlayerButtonSelected];
}

void DrawMainMenu() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
  DrawText("Main Menu", 20, 20, 60, MAROON);

  DrawRectangleRounded(practiceButtonRec, buttonRoundness, buttonSegments,
                       practiceButtonColor);
  DrawText("Practice Mode", practiceButtonRec.x, practiceButtonRec.y, 60,
           practiceButtonTextColor);

  DrawRectangleRounded(twoPlayerButtonRec, buttonRoundness, buttonSegments,
                       twoPlayerButtonColor);
  DrawText("Two Player Mode", twoPlayerButtonRec.x, twoPlayerButtonRec.y, 60,
           twoPlayerButtonTextColor);
}
