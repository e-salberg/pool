#pragma once

#define MAX_BALLS 16
#define BALL_MASS 10
#define BALL_RADIUS 25

typedef enum { Q, SOLID, STRIPE, EIGHT } BALL_TYPE;

void InitPracticeScene();
void UpdatePracticeScene(float deltaTime);
void DrawPracticeScene();
