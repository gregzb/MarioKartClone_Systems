#ifndef COLLISIONS_INCLUDE
#define COLLISIONS_INCLUDE

#include "vec2.h"
#include "kart.h"
#include "level.h"

bool check_terrain_collisions(struct kart * cart, SDL_Rect * terrain_rect, SDL_Rect * resultant);

#endif