#include "vec2.h"
#include <math.h>
#include <stdio.h>

vec2 v2_neg(vec2 a) {
  return (vec2) {-a.x, -a.y};
}

double v2_mag(vec2 a) {
  return hypot(a.x, a.y);
}

double v2_mag_sqr(vec2 a) {
  return a.x * a.x + a.y * a.y;
}

double v2_angle(vec2 a) {
  return atan2(a.y, a.x);
}

double v2_angle_between(vec2 a, vec2 b) {
  return acos(v2_dot(a, b) / (double) (v2_mag(a) * v2_mag(b)));
}

vec2 v2_add(vec2 a, vec2 b) {
  return (vec2) {a.x+b.x, a.y+b.y};
}

vec2 v2_sub(vec2 a, vec2 b) {
  return (vec2) {a.x-b.x, a.y-b.y};
}

double v2_dot(vec2 a, vec2 b) {
  return a.x*b.x + a.y+b.y;
}

double v2_cross(vec2 a, vec2 b) {
  return a.x*b.y - a.y*b.x;
}

char v2_epsilon_equals(double a, double b, double epsilon) {
  return fabs(a - b) < epsilon;
}

char v2_equal(vec2 a, vec2 b) {
  return v2_epsilon_equals(a.x, b.x, 0.000001) && v2_epsilon_equals(a.y, b.y, 0.000001);
}

vec2 v2_rotate(vec2 a, double angle) {
  double co = cos(angle);
  double si = sin(angle);
  return (vec2) {co*a.x - si*a.y, si*a.x + co*a.y};
}

void v2_print(vec2 a) {
  printf("<%lf, %lf>", a.x, a.y);
}
