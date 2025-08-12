#include "practice.h"
#include "ball.h"
#include "raylib.h"
#include "table.h"

static Ball physics[MAX_BALLS] = {0};
static bool onTable[MAX_BALLS];
static BALL_TYPE types[MAX_BALLS] = {
    Q,     SOLID,  SOLID,  SOLID,  SOLID,  SOLID,  SOLID,  SOLID,
    EIGHT, STRIPE, STRIPE, STRIPE, STRIPE, STRIPE, STRIPE, STRIPE};
static Color colors[MAX_BALLS] = {
    WHITE, YELLOW, BLUE, RED, PURPLE, ORANGE, GREEN, DARKBROWN,
    BLACK, YELLOW, BLUE, RED, PURPLE, ORANGE, GREEN, DARKBROWN};
static int startingOrder[MAX_BALLS] = {0, 1,  11, 5, 2,  8,  10, 9,
                                       4, 14, 7,  6, 15, 13, 3,  12};

typedef enum {
  // BREAK,
  TURN_START,
  BALLS_IN_MOTION,
  SCRATCHED,
} PracticeState;

static PracticeState state;
static bool canShoot;
static bool hasScratched;
static bool canPlaceQ;
static Vector2 mousePosition = {0};

void InitPracticeScene() {
  canShoot = true;
  hasScratched = false;
  canPlaceQ = true;
  mousePosition.x = 0;
  mousePosition.y = 0;

  InitTable();
  for (int i = 0; i < MAX_BALLS; i++) {
    physics[i].velocity.x = 0.0f;
    physics[i].velocity.y = 0.0f;
    physics[i].mass = BALL_MASS;
    physics[i].radius = BALL_RADIUS;
    onTable[i] = true;
  }

  // TODO - placement should be based on table not screen
  Rectangle table = getTable();
  int COLS = 5;
  int idx = 1;
  for (int col = 0; col < COLS; col++) {
    float baseHeight = (table.y + table.height / 2) - (col * BALL_RADIUS);
    for (int i = 0; i <= col; i++) {
      int ball = startingOrder[idx];
      physics[ball].position.x =
          (table.x + table.width * .6f) + (col * 1.75 * BALL_RADIUS);
      physics[ball].position.y = baseHeight + (i * 2 * BALL_RADIUS);
      idx++;
    }
  }
  physics[0].position.x = table.x + table.width / 4;
  physics[0].position.y = table.y + table.height / 2;
}

void UpdatePracticeScene(float deltaTime) {
  mousePosition = GetMousePosition();
  switch (state) {
  case TURN_START:
    onTable[0] = true;
    if (IsGestureDetected(GESTURE_TAP)) {
      physics[0].velocity.x = 3 * (physics[0].position.x - mousePosition.x);
      physics[0].velocity.y = 3 * (physics[0].position.y - mousePosition.y);
      state = BALLS_IN_MOTION;
    }
    break;
  case BALLS_IN_MOTION:
    for (int i = 0; i < MAX_BALLS; i++) {
      if (!onTable[i]) {
        continue;
      }
      UpdateBallPosition(&physics[i], deltaTime);
      if (checkBallWentInPocket(physics[i])) {
        onTable[i] = false;
        if (types[i] == Q) {
          hasScratched = true;
          Rectangle table = getTable();
          physics[0].position.x = table.x + table.width / 6;
          physics[0].position.y = table.y + table.height / 2;
          physics[0].velocity.x = 0;
          physics[0].velocity.y = 0;
        }
      }
    }

    for (int i = 0; i < MAX_BALLS; i++) {
      if (!onTable[i]) {
        continue;
      }
      handleBallToWallCollision(&physics[i]);
      for (int j = i + 1; j < MAX_BALLS; j++) {
        if (!onTable[j]) {
          continue;
        }
        if (hasCollided(physics[i], physics[j])) {
          handleBallToBallCollision(&physics[i], &physics[j]);
        }
      }
    }

    bool allBallsStoppedMoving = true;
    for (int i = 0; i < MAX_BALLS; i++) {
      if (!onTable[i]) {
        continue;
      }
      if (physics[i].velocity.x != 0 || physics[i].velocity.y != 0) {
        allBallsStoppedMoving = false;
        UpdateBallVelocity(&physics[i], deltaTime);
      }
    }
    if (allBallsStoppedMoving) {
      state = TURN_START;
      if (hasScratched) {
        state = SCRATCHED;
        onTable[0] = true;
      }
    }
    break;
  case SCRATCHED:
    canPlaceQ = true;
    physics[0].position = ResetBallAfterScratch(physics[0]);
    for (int i = 1; i < MAX_BALLS; i++) {
      if (hasCollided(physics[0], physics[i])) {
        // TODO - move ball so it's no longer colliding
        // issue is with multiple balls close to each other
        canPlaceQ = false;
      }
    }
    if (checkBallWentInPocket(physics[0])) {
      canPlaceQ = false;
    }

    if (canPlaceQ && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      state = TURN_START;
      hasScratched = false;
    }
    break;
  }
}

void UpdatePracticeScene_old(float deltaTime) {
  mousePosition = GetMousePosition();

  if (hasScratched && canShoot) {
    physics[0].position = ResetBallAfterScratch(physics[0]);
    canPlaceQ = true;
    for (int i = 1; i < MAX_BALLS; i++) {
      if (hasCollided(physics[0], physics[i])) {
        // TODO - move ball so it's no longer colliding
        // issue is with multiple balls close to each other
        canPlaceQ = false;
      }
    }
    if (checkBallWentInPocket(physics[0])) {
      canPlaceQ = false;
    }

    if (canPlaceQ && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      hasScratched = false;
    }
    return;
  }

  if (canShoot && IsGestureDetected(GESTURE_TAP)) {
    canShoot = false;
    physics[0].velocity.x = 3 * (physics[0].position.x - mousePosition.x);
    physics[0].velocity.y = 3 * (physics[0].position.y - mousePosition.y);
  }

  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    }
    UpdateBallPosition(&physics[i], deltaTime);
  }

  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    }
    for (int j = i + 1; j < MAX_BALLS; j++) {
      if (!onTable[j]) {
        continue;
      }
      if (hasCollided(physics[i], physics[j])) {
        handleBallToBallCollision(&physics[i], &physics[j]);
      }
    }
  }

  bool allBallsStoppedMoving = true;
  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    } else if (checkBallWentInPocket(physics[i])) {
      onTable[i] = false;
      hasScratched = types[i] == Q;
      if (hasScratched) {
        Rectangle table = getTable();
        physics[0].position.x = table.x + table.width / 6;
        physics[0].position.y = table.y + table.height / 2;
        physics[0].velocity.x = 0;
        physics[0].velocity.y = 0;
      }
      continue;
    }

    handleBallToWallCollision(&physics[i]);
    if (physics[i].velocity.x != 0 || physics[i].velocity.y != 0) {
      allBallsStoppedMoving = false;
      // accelerateBall(&physics[i], deltaTime);
    }
  }

  if (allBallsStoppedMoving) {
    canShoot = true;
    onTable[0] = true;
  }
}

void DrawPracticeScene() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE);
  DrawTable();

  switch (state) {
  case TURN_START:
    DrawText(TextFormat("Start"), 10, 40, 40, BLACK);
    break;
  case BALLS_IN_MOTION:
    DrawText(TextFormat("Motion"), 10, 40, 40, BLACK);
    break;
  case SCRATCHED:
    DrawText(TextFormat("Scratched"), 10, 40, 40, BLACK);
    break;
  }

  if (canShoot) {
    DrawText(TextFormat("PLAYER TURN"), 400, 40, 60, BLACK);
  }
  if (hasScratched && !canPlaceQ) {
    DrawText(TextFormat("CAN'T PLACE Q HERE"), 400, 100, 60, RED);
  }

  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    }
    DrawBall(physics[i], types[i], colors[i]);
  }
  DrawFPS(10, 10);
}
