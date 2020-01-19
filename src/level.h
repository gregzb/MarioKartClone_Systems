#pragma once

#include <SDL2/SDL.h>

struct level {
  SDL_Texture * level_image;
  SDL_Point size;
  SDL_Rect * collision_boxes;
  int num_boxes;
  double scale_factor;
};

struct level level_init(SDL_Renderer* renderer, char* level_name);
