#include "sdl_utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture *load_image(SDL_Renderer *renderer, char *file_name)
{
  SDL_Surface *surface = SDL_LoadBMP(file_name);
  if (surface == NULL)
  {
    printf("Failed to load image %s error: %s\n", file_name, SDL_GetError());
    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  return texture;
}

SDL_Texture *surface_to_texture(SDL_Renderer *renderer, SDL_Surface *surf)
{
  SDL_Texture *text;
  text = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);
  return text;
}

char render_button(SDL_Renderer *renderer, SDL_Rect *bounds, SDL_Color color, TTF_Font *font, char *text, SDL_Point *mouse_pos, int text_height)
{
  char temp = 0;
  if (SDL_PointInRect(mouse_pos, bounds))
  {
    color.r *= .8;
    color.g *= .8;
    color.b *= .8;
    temp = 1;
  }
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  SDL_RenderFillRect(renderer, bounds);

  SDL_Surface *solid = TTF_RenderText_Solid(font, text, (SDL_Color){255, 255, 255, 255});
  SDL_Texture *text_texture = surface_to_texture(renderer, solid);

  SDL_Rect text_bounds = {0, 0, 0, 0};

  SDL_QueryTexture(text_texture, NULL, NULL, &text_bounds.w, &text_bounds.h);
  double asp_ratio = text_bounds.h / (double)text_bounds.w;

  SDL_Rect text_rect = *bounds;
  text_rect.x += 10;
  text_rect.y += 10;
  text_rect.h = text_height;
  text_rect.w = (int)(text_rect.h * (1 / asp_ratio));

  SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
  SDL_DestroyTexture(text_texture);
  return temp;
}

void render_text(SDL_Renderer *renderer, TTF_Font* font, SDL_Point pos, int text_height, char *text, SDL_Color color)
{
  SDL_Surface *solid = TTF_RenderText_Solid(font, text, color);
  SDL_Texture *text_texture = surface_to_texture(renderer, solid);

  SDL_Rect text_bounds = {0};
  SDL_QueryTexture(text_texture, NULL, NULL, &text_bounds.w, &text_bounds.h);
  double asp_ratio = text_bounds.h / (double)text_bounds.w;
  text_bounds.h = text_height;
  text_bounds.w = (int)(text_bounds.h * (1 / asp_ratio));
  text_bounds.x = pos.x - text_bounds.w / 2;
  text_bounds.y = pos.y - text_bounds.h / 2;

  SDL_RenderCopy(renderer, text_texture, NULL, &text_bounds);
  SDL_DestroyTexture(text_texture);
}