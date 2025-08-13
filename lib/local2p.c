#include "local2p.h"
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
  GAMEOVER,
} Local2pState;

static int currentPlayer = 0;
static BALL_TYPE playerAssignedBallType[2] = {NONE, NONE};

static Local2pState state;
static bool playerScoredCorrectType;
static bool hasScratched;
static bool canPlaceQ;
static Vector2 mousePosition = {0};

void InitLocal2pScene() {
  hasScratched = false;
  canPlaceQ = true;
  mousePosition.x = 0;
  mousePosition.y = 0;
  state = TURN_START;

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

void UpdateLocal2pScene(float deltaTime) {
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
        } else if (types[i] == EIGHT) {
          // if current player has all other balls in hole they win
          // else they lose
          state = GAMEOVER;
        } else {
          if (playerAssignedBallType[currentPlayer] == NONE) {
            playerAssignedBallType[currentPlayer] = types[i];
            playerAssignedBallType[!currentPlayer] =
                types[i] == SOLID ? STRIPE : SOLID;
          }
          if (playerAssignedBallType[currentPlayer] == types[i]) {
            playerScoredCorrectType = true;
          }
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
        currentPlayer = !currentPlayer;
      } else if (!playerScoredCorrectType) {
        currentPlayer = !currentPlayer;
      }
      playerScoredCorrectType = false;
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
  case GAMEOVER:
    break;
  }
}

void DrawLocal2pScene() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE);
  DrawTable();

  for (int i = 0; i < MAX_BALLS; i++) {
    if (!onTable[i]) {
      continue;
    }
    DrawBall(physics[i], types[i], colors[i]);
  }

  Rectangle p1ScoreBox = {.x = (float)GetScreenWidth() / 8,
                          .y = (float)GetScreenHeight() * 4 / 5,
                          .width = 500,
                          .height = 250};
  Rectangle p2ScoreBox = {.x = (float)GetScreenWidth() * 5 / 8,
                          .y = (float)GetScreenHeight() * 4 / 5,
                          .width = 500,
                          .height = 250};
  DrawRectangleRec(p1ScoreBox, GRAY);
  DrawRectangleRec(p2ScoreBox, GRAY);

  int p1Score = 0;
  int p2Score = 0;
  for (int i = 0; i < MAX_BALLS; i++) {
    if (types[i] == Q || types[i] == EIGHT || onTable[i]) {
      continue;
    }
    // p1Score += types[i] == playerAssignedBallType[0];
    // p2Score += types[i] == playerAssignedBallType[1];
    if (types[i] == playerAssignedBallType[0]) {
      p1Score++;
      physics[i].position.x = p1ScoreBox.x + physics[i].radius +
                              (2 * physics[i].radius * p1Score) + (5 * p1Score);
      physics[i].position.y = p1ScoreBox.y + p1ScoreBox.height / 2;
    } else {
      p2Score++;
      physics[i].position.x = p2ScoreBox.x + physics[i].radius +
                              (2 * physics[i].radius * p2Score) + (5 * p2Score);
      physics[i].position.y = p2ScoreBox.y + p2ScoreBox.height / 2;
    }
    DrawBall(physics[i], types[i], colors[i]);
  }
  Vector2 textOffset = {85, 15};
  DrawText(TextFormat("Player 1: %i", p1Score), p1ScoreBox.x + textOffset.x,
           p1ScoreBox.y + textOffset.y, 60, BLACK);
  DrawText(TextFormat("Player 2: %i", p2Score), p2ScoreBox.x + textOffset.x,
           p2ScoreBox.y + textOffset.y, 60, BLACK);

  int currentPlayerScore = currentPlayer ? p2Score : p1Score;
  int winner = currentPlayerScore == 7 ? currentPlayer : !currentPlayer;
  switch (state) {
  case TURN_START:
    DrawText(TextFormat("Start"), 10, 40, 40, BLACK);
    break;
  case BALLS_IN_MOTION:
    DrawText(TextFormat("Motion"), 10, 40, 40, BLACK);
    break;
  case SCRATCHED:
    DrawText(TextFormat("Scratched"), 10, 40, 40, BLACK);
    if (!canPlaceQ) {
      DrawText(TextFormat("CAN'T PLACE Q HERE"), 400, 100, 60, RED);
    }
    break;
  case GAMEOVER:
    DrawText(TextFormat("GAMEOVER"), 10, 40, 40, BLACK);
    DrawText(TextFormat("Player %i Won!", winner + 1), 400, 100, 60, RED);
    break;
  }

  if (state == TURN_START || state == SCRATCHED) {
    DrawText(TextFormat("PLAYER %i's TURN", currentPlayer + 1), 400, 40, 60,
             !currentPlayer ? BLACK : RED);
  }

  DrawFPS(10, 10);
}
