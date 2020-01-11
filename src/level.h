#pragma once

#include <SDL2/SDL.h>

struct level {
  SDL_Texture * level_image;
  SDL_Rect * collision_boxes;
};

struct level level_init(SDL_Renderer* renderer, char* level_name);
