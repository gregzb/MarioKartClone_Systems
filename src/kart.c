#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "kart.h"
#include "vec2.h"

// const double CONSTANT_ACCEL = 100;//REMEMBER TO SET THESE PROPERLY BEFORE STARTING
// const double MAX_ACCEL = 100000; //REMEMBER TO

const double MAX_VELOCITY = 3;
const double CONSTANT_ACCEL = 10;
const double ROTATION_SPEED = 2.25;
const double DRAG = 0.065;

struct kart kart_init()
{
  struct kart temp;
  temp.direction = (vec2){0, -1};
  temp.position = (vec2){0};
  temp.velocity = (vec2){0};
  temp.acceleration = (vec2){0};
  temp.size = (SDL_Point) {15, 15};
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
  current_kart->velocity = v2_mult(current_kart->velocity, 1 - DRAG);
  if (v2_mag(current_kart->velocity) > dt * 20 || 1)
  {
    kart_update_direction(current_kart, lr, dt);
  }
  vec2 accel = v2_mult(v2_normalize(current_kart->direction), CONSTANT_ACCEL * acc * dt);
  current_kart->acceleration = accel;
  kart_update_velocity(current_kart);
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

void kart_update_velocity(struct kart *current_kart)
{
  //current_kart->velocity = v2_add(current_kart->velocity, v2_mult(current_kart->acceleration, dt));
  current_kart->velocity = v2_add(current_kart->velocity, current_kart->acceleration);
  if (v2_mag(current_kart->velocity) > MAX_VELOCITY)
  {
    current_kart->velocity = v2_mult(v2_normalize(current_kart->velocity), MAX_VELOCITY);
  }
}

void kart_reverse_direction(struct kart * current_kart)
{
  current_kart -> direction = v2_rotate(current_kart -> direction, 3.14);
}

void kart_reverse_velocity(struct kart * current_kart)
{
  current_kart -> velocity = v2_mult(current_kart -> velocity, -1);
}