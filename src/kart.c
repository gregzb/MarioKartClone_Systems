#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "kart.h"
#include "vec2.h"

// const double CONSTANT_ACCEL = 100;//REMEMBER TO SET THESE PROPERLY BEFORE STARTING
// const double MAX_ACCEL = 100000; //REMEMBER TO

struct kart kart_init(){
  struct kart temp;
  temp.direction = (vec2) {0};
  temp.position = (vec2) {0};
  temp.velocity = (vec2) {0};
  temp.acceleration = (vec2) {0};
  return temp;
}

void kart_set_direction(struct kart * kart, double x, double y){
  kart->direction.x = x;
  kart->direction.y = y;
}

void kart_set_position(struct kart * kart, double x, double y){
  kart->position.x = x;
  kart->position.y = y;
}

void kart_set_velocity(struct kart * kart, double x, double y){
  kart->velocity.x = x;
  kart->velocity.y = y;
}

void kart_set_acceleration(struct kart * kart, double x, double y){
  kart->acceleration.x = x;
  kart->acceleration.y = y;
}

void kart_move(struct kart * current_kart, double acc, char lr, double dt){// lr should be -1 for left, 1 for right, 0 for neither
  kart_update_direction(current_kart, lr);
  //set_acceleration(current_kart, sin(v2_angle(current_kart->direction)) * acc, cos(v2_angle(current_kart->direction)) * acc);
  vec2 accel = v2_mult(v2_normalize(current_kart->direction), acc);
  kart_set_acceleration(current_kart, accel.x, accel.y);
  kart_update_velocity(current_kart, dt);
  kart_update_position(current_kart, dt);
}

void kart_update_direction(struct kart * current_kart, char lr) {
  current_kart->direction = v2_rotate(current_kart->direction, lr * 0.0174533);
}

void kart_update_position(struct kart * current_kart, double dt) {
  current_kart->position = v2_add(current_kart->position, v2_mult(current_kart->velocity, dt));
}

void kart_update_velocity(struct kart * current_kart, double dt) {
  current_kart->velocity = v2_add(current_kart->velocity, v2_mult(current_kart->acceleration, dt));
  if (v2_mag(current_kart->velocity) > 50) {
    current_kart->velocity = v2_mult(v2_normalize(current_kart->velocity), 50);
  }
}

/***
struct kart update_acceleration(struct kart current_kart, int xdir, int ydir){//negative number for down, left, position for up, right, 0 for neither
  if (xdir == 0 || ydir == 0){
    current_kart.acceleration.x = add_acceleration(current_kart.acceleration.x, CONSTANT_ACCEL, ydir);
    current_kart.acceleration.y = add_acceleration(current_kart.acceleration.y, CONSTANT_ACCEL, xdir);
  }else{
    current_kart.acceleration.x = add_acceleration(current_kart.acceleration.x, sqrt(CONSTANT_ACCEL) / 2, ydir);
    current_kart.acceleration.y = add_acceleration(current_kart.acceleration.y, sqrt(CONSTANT_ACCEL) / 2, xdir);
  }
  return current_kart;
}

int add_acceleration(double currentA, double addition, int dir){
  double newA = currentA;
  if (dir < 0){
    newA = currentA - addition;
  }if (dir > 0){
    newA = currentA + addition;
  }
  return newA;
}

***/
