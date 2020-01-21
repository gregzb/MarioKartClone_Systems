#include "sdl_utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "vec2.h"
#include "math.h"

struct v2_rect v2_rect_from_vecs(vec2 top_left, vec2 bottom_right)
{
    struct v2_rect temp = {0};
    temp.x = top_left.x;
    temp.y = top_left.y;
    temp.w = bottom_right.x - top_left.x;
    temp.h = bottom_right.x - top_left.y;
    return temp;
}

char v2_rect_intersection(struct v2_rect rect1, struct v2_rect rect2, struct v2_rect *intersection)
{
    if (rect1.x < rect2.x + rect2.w &&
        rect1.x + rect1.w > rect2.x &&
        rect1.y < rect2.y + rect2.h &&
        rect1.y + rect1.h > rect2.y)
    {
        intersection->x = fmax(rect1.x, rect2.x);
        intersection->y = fmax(rect1.y, rect2.y);
        intersection->w = fmin(rect1.x + rect1.w, rect2.x + rect2.w) - intersection->x;
        intersection->h = fmin(rect1.y + rect1.h, rect2.y + rect2.h) - intersection->y;
        return 1;
    }
    return 0;
}

struct v2_rect v2_rect_from_SDL_Rect(SDL_Rect rect)
{
    struct v2_rect temp = {rect.x, rect.y, rect.w, rect.h};
    return temp;
}

void v2_rect_print(struct v2_rect rect)
{
    printf("r{%lf, %lf, %lf, %lf}\n", rect.x, rect.y, rect.w, rect.h);
}

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

void render_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Point pos, int text_height, char *text, SDL_Color color)
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