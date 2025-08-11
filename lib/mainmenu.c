#include "mainmenu.h"
#include "raylib.h"

void UpdateMainMenu() {}

void DrawMainMenu() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
  DrawText("Main Menu", 20, 20, 60, MAROON);
  DrawText("PRESS ENTER or TAP to START", 225, GetScreenHeight() / 3, 80,
           MAROON);
}
