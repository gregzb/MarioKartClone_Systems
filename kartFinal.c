#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "kartFinal.h"
#include "vec2.h"
#include "vec2.c"

// const double CONSTANT_ACCEL = 100;//REMEMBER TO SET THESE PROPERLY BEFORE STARTING
// const double MAX_ACCEL = 100000; //REMEMBER TO

struct kart initialize(){
  struct kart A;
  A.direction.x = 0;
  A.direction.y = 0;
  A.position.x = 0;
  A.position.y = 0;
  A.velocity.x = 0;
  A.velocity.y = 0;
  A.acceleration.x = 0;
  A.acceleration.y = 0;
  return A;
}

struct kart set_direction(struct kart cart, double x, double y){
  cart.direction.x = x;
  cart.direction.y = y;
  return cart;
}

struct kart set_position(struct kart cart, double x, double y){
  cart.position.x = x;
  cart.position.y = y;
  return cart;
}

struct kart set_acceleration(struct kart cart, double x, double y){
  cart.acceleration.x = x;
  cart.acceleration.y = y;
  return cart;
}
struct kart move(struct kart current_kart, double acc, int lr, double timePassed){// lr should be -1 for left, 1 for right, 0 for neither
  //current_kart = update_acceleration(current_kart, xdir, ydir);
  current_kart = update_direction(current_kart, lr);
  current_kart = set_acceleration(current_kart, sin(v2_angle(current_kart.direction)) * acc, cos(v2_angle(current_kart.direction)) * acc);
  current_kart = update_velocity(current_kart, timePassed);
  current_kart = update_position(current_kart, timePassed);
  return current_kart;
}

struct kart update_direction(struct kart * current_kart, double lr){
  current_kart.direction = v2_rotate(current_kart.direction, lr * 0.0174533);
  return current_kart;
}

struct kart update_position(struct kart current_kart, double timePassed){
  current_kart.position = vec2_addition( current_kart.position, scalar_mult(current_kart.velocity, timePassed));
  return current_kart;
}

struct kart update_velocity(struct kart current_kart, double timePassed){
  current_kart.velocity = vec2_addition(current_kart.velocity, scalar_mult(current_kart.acceleration, timePassed));
  current_kart.velocity = scalar_mult(vec2_normalize(current_kart.velocity), 50);
  return current_kart;
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

struct vec2 vec2_addition(struct vec2 A, struct vec2 B){
  struct vec2 total;
  total.x = A.x + B.x;
  total.y = A.y + B.y;
  return total;
}

struct vec2 scalar_mult(struct vec2 A, double multiplier){
  A.x = A.x * multiplier;
  A.y = A.y * multiplier;
  return A;
}
