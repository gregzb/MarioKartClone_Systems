#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <math.h>

#include "kart.h"
#include "vec2.h"
#include "sdl_utils.h"

// const double CONSTANT_ACCEL = 100;//REMEMBER TO SET THESE PROPERLY BEFORE STARTING
// const double MAX_ACCEL = 100000; //REMEMBER TO

const double MAX_VELOCITY = 3;
const double CONSTANT_ACCEL = 10;
const double ROTATION_SPEED = 2.25;
const double DRAG = 0.065;

struct kart kart_init(struct level *lvl)
{
  struct kart temp = {0};
  temp.direction = (vec2){0, -1};
  temp.position = (vec2){0};
  temp.velocity = (vec2){0};
  temp.acceleration = (vec2){0};
  temp.size = (SDL_Point){15, 15};
  return temp;
}

// void kart_set_direction(struct kart * kart, double x, double y){
//   kart->direction.x = x;
//   kart->direction.y = y;
// }
//
// void kart_set_position(struct kart * kart, double x, double y){
//   kart->position.x = x;
//   kart->position.y = y;
// }
//
// void kart_set_velocity(struct kart * kart, double x, double y){
//   kart->velocity.x = x;
//   kart->velocity.y = y;
// }
//
// void kart_set_acceleration(struct kart * kart, double x, double y){
//   kart->acceleration.x = x;
//   kart->acceleration.y = y;
// }

void kart_move(struct kart *current_kart, char acc, char lr, double dt)
{ // lr should be -1 for left, 1 for right, 0 for neither
  //printf("%lf %lf\n", v2_mag(current_kart->velocity), dt);
  //printf("doot: %f %f\n", (1-DRAG), (1 - DRAG * (dt*60)));
  current_kart->velocity = v2_mult(current_kart->velocity, (1 - DRAG * (dt * 60)));
  if (v2_mag(current_kart->velocity) > dt * 20)
  {
    kart_update_direction(current_kart, lr, dt);
  }
  vec2 accel = v2_mult(v2_normalize(current_kart->direction), CONSTANT_ACCEL * acc * dt);
  current_kart->acceleration = accel;
  kart_update_velocity(current_kart, dt);
  kart_update_position(current_kart);
}

void kart_update_direction(struct kart *current_kart, char lr, double dt)
{
  //1 degree
  //current_kart->direction = v2_rotate(current_kart->direction, lr * dt * 0.0174533);
  current_kart->direction = v2_rotate(current_kart->direction, lr * dt * ROTATION_SPEED);
}

void kart_update_position(struct kart *current_kart)
{
  //current_kart->position = v2_add(current_kart->position, v2_mult(current_kart->velocity, dt));
  current_kart->position = v2_add(current_kart->position, current_kart->velocity);
}

void kart_update_velocity(struct kart *current_kart, double dt)
{
  //current_kart->velocity = v2_add(current_kart->velocity, v2_mult(current_kart->acceleration, dt));
  current_kart->velocity = v2_add(current_kart->velocity, current_kart->acceleration);
  //printf("%f %f\n", MAX_VELOCITY, MAX_VELOCITY * (dt*60));
  if (v2_mag(current_kart->velocity) > MAX_VELOCITY * (dt * 60))
  {
    current_kart->velocity = v2_mult(v2_normalize(current_kart->velocity), MAX_VELOCITY * (dt * 60));
  }
}

void kart_handle_collision(struct kart *current_kart, SDL_Rect *rect, double dt)
{
  struct v2_rect kart_rect = {current_kart->position.x, current_kart->position.y, current_kart->size.x, current_kart->size.y};
  kart_rect.x -= kart_rect.w / 2;
  kart_rect.y -= kart_rect.h / 2;

  struct v2_rect other_rect = {rect->x, rect->y, rect->w, rect->h};

  struct v2_rect intersection = {0};

  //printf("%d, %d, %d, %d, %f, %f\n", rect->x, rect->y, rect->w, rect->h, current_kart->position.x, current_kart->position.y);

  char intersected = v2_rect_intersection(kart_rect, other_rect, &intersection);

  if (!intersected)
    return;

  vec2 avg_intersect = {intersection.x + intersection.w / 2, intersection.y + intersection.h / 2};
  vec2 avg_pos = {kart_rect.x + kart_rect.w / 2, kart_rect.y + kart_rect.h / 2};

  vec2 push_vec = v2_sub(avg_pos, avg_intersect);
  if (abs(push_vec.y) > abs(push_vec.x))
  {
    push_vec.x = 0;
  }
  else
  {
    push_vec.y = 0;
  }
  vec2 normed = v2_normalize(push_vec);
  normed.x *= intersection.w;
  normed.y *= intersection.h;
  current_kart->position = v2_add(current_kart->position, normed);
}

void kart_reverse_direction(struct kart *current_kart)
{
  current_kart->direction = v2_rotate(current_kart->direction, 3.14);
}

void kart_reverse_velocity(struct kart *current_kart)
{
  current_kart->velocity = v2_mult(current_kart->velocity, -1);
}