#pragma once

#include <SDL2/SDL.h>

struct level {
  //will be NULL if renderer is NULL in level_init
  SDL_Texture * level_image;
  SDL_Point size;
  SDL_Rect * collision_boxes;
  int num_boxes;
  double scale_factor;
};

//renderer may be NULL, indicating no level image will be loaded
struct level level_init(SDL_Renderer* renderer, char* level_name);
