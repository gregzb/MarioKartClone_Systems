#pragma once

struct vec2 {
  double x;
  double y;
};

typedef struct vec2 vec2;

vec2 v2_neg(vec2 a);

double v2_mag(vec2 a);

double v2_mag_sqr(vec2 a);

double v2_angle(vec2 a);

double v2_angle_between(vec2 a, vec2 b);

vec2 v2_add(vec2 a, vec2 b);

vec2 v2_sub(vec2 a, vec2 b);

vec2 v2_mult(vec2 a, double b);

double v2_dot(vec2 a, vec2 b);

double v2_cross(vec2 a, vec2 b);

//1 is true, 0 is false, epsilon is smthing
char v2_equal(vec2 a, vec2 b);

//angle is in rad
vec2 v2_rotate(vec2 a, double angle);

vec2 v2_normalize(vec2 a);

void v2_print(vec2 a);
