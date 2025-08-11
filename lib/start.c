#include "start.h"
#include "raylib.h"
#include <math.h>

static double currentAlpha = 255;

void UpdateStartScene() {
  currentAlpha = (.7 + .7 * sin(GetTime() * 2)) * 255;
  currentAlpha = currentAlpha > 255 ? 255 : currentAlpha;
}

void DrawStartScene() {
  Color textColor = {0, 82, 172, currentAlpha};
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
  DrawText("PRESS ENTER or TAP to START", 225, GetScreenHeight() / 3, 80,
           textColor);
}
