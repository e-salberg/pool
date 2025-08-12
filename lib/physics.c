#include "physics.h"
#include <math.h>

float sqr(float x) { return x * x; }

float distance(Vector2 a, Vector2 b) {
  return sqrtf(sqr(a.x - b.x) + sqr(a.y - b.y));
}

float magnitude(Vector2 vec) {
  return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

Vector2 normalize(Vector2 vec) {
  Vector2 nv = {0};
  float mag = magnitude(vec);
  if (mag) {
    nv.x = vec.x / mag;
    nv.y = vec.y / mag;
  }
  return nv;
}

float dot_product(Vector2 a, Vector2 b) { return (a.x * b.x) + (a.y * b.y); }
