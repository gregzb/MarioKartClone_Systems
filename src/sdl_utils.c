#include "sdl_utils.h"

#include <SDL2/SDL.h>
SDL_Texture* load_image(SDL_Renderer* renderer, char* file_name)
{
  SDL_Surface* surface = SDL_LoadBMP(file_name);
  if (surface == NULL) {
    printf("Failed to load image %s error: %s\n", file_name, SDL_GetError());
    return NULL;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );
  SDL_FreeSurface( surface );

  return texture;
}

SDL_Texture* surface_to_texture(SDL_Renderer* renderer, SDL_Surface* surf )
{
	SDL_Texture* text;
	text = SDL_CreateTextureFromSurface( renderer, surf );
	SDL_FreeSurface( surf );
	return text;
}
