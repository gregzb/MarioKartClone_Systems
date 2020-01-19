#pragma once

#include <SDL2/SDL.h>

struct level {
  SDL_Texture * level_image;
  SDL_Point size;
  SDL_Rect * collision_boxes;
  int num_boxes;
  SDL_Point spawn_points[4];
  SDL_Rect * start_boxes;
  int num_start_boxes;
  SDL_Rect * cp_1;
  int num_cp_1;
  SDL_Rect * cp_2;
  int num_cp_2;
  double scale_factor;
};

struct level level_init(SDL_Renderer* renderer, char* level_name);
