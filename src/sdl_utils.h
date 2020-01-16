#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* load_image(SDL_Renderer* renderer, char* file_name);
SDL_Texture* surface_to_texture(SDL_Renderer* renderer, SDL_Surface* surf );
char render_button(SDL_Renderer* renderer, SDL_Rect* bounds, SDL_Color color, TTF_Font* font, char* text, SDL_Point* mouse_pos, int text_height);
void render_text(SDL_Renderer *renderer, TTF_Font* font, SDL_Point pos, int text_height, char *text, SDL_Color color);