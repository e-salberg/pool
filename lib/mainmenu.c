#include "mainmenu.h"
#include "raylib.h"

static float buttonRoundness = 0.2f;
static float buttonWidth;  // = 200.0f;
static float buttonHeight; // = 100.0f;
static float buttonSegments = 0.0f;
static float buttonLineThick = 1.0f;

static Rectangle practiceButtonRec;
static Rectangle twoPlayerButtonRec;

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

void UpdateMainMenu() {}

void DrawMainMenu() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
  DrawText("Main Menu", 20, 20, 60, MAROON);

  DrawRectangleRounded(practiceButtonRec, buttonRoundness, buttonSegments,
                       GRAY);
  DrawText("Practice Mode", practiceButtonRec.x, practiceButtonRec.y, 60,
           MAROON);

  DrawRectangleRounded(twoPlayerButtonRec, buttonRoundness, buttonSegments,
                       GRAY);
  DrawText("Two Player Mode", twoPlayerButtonRec.x, twoPlayerButtonRec.y, 60,
           MAROON);
}
