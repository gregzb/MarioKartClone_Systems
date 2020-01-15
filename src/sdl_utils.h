#pragma once

#include <SDL2/SDL.h>

SDL_Texture* load_image(SDL_Renderer* renderer, char* file_name);
SDL_Texture* surface_to_texture(SDL_Renderer* renderer, SDL_Surface* surf );
