#ifndef KART_INCLUDE
#define KART_INCLUDE

#define _GNU_SOURCE

#include <SDL2/SDL.h>

#include "vec2.h"


struct kart {
  struct vec2 direction;
  struct vec2 position;
  struct vec2 velocity;
  struct vec2 acceleration;
  SDL_Point size;
};


struct kart kart_init();
// void kart_set_direction(struct kart * cart, double x, double y);
// void kart_set_position(struct kart * cart, double x, double y);
// void kart_set_velocity(struct kart * cart, double x, double y);
// void kart_set_acceleration(struct kart * cart, double x, double y);
void kart_move(struct kart * current_kart, char acc, char lr, double dt);
void kart_update_direction(struct kart * current_kart, char lr, double dt);
void kart_update_position(struct kart * current_kart);
void kart_update_velocity(struct kart * current_kart);
void kart_reverse_direction(struct kart * current_kart);
void kart_reverse_velocity(struct kart * current_kart);
void set_velocity(struct kart *current_kart, vec2 velocity);
void kart_set_direction(struct kart *current_kart, vec2 direction);
// struct kart update_acceleration(struct kart current_kart, int xdir, int ydir);//negative number for down, left, position for up, right, 0 for neither
// int add_acceleration(double currentA, double addition, int dir);

#endif
