
#define _GNU_SOURCE

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

#include "vec2.h"
#include "time_util.h"
#include "server.h"
#include "collisions.h"
#include "kart.h"
#include "level.h"
#include "sdl_utils.h"
#include "networking.h"




bool check_terrain_collisions(struct kart * kart, SDL_Rect * terrain_rect, SDL_Rect * resultant) {
    SDL_Rect temp = {kart -> position.x, kart -> position.y, kart->size.x, kart->size.y};
    if (SDL_IntersectRect(&temp, terrain_rect, resultant)){
        if (kart -> direction.x == 0 || kart -> direction.y == 0)//SDL CODE FOR CHECKING IF ITS HEAD ON, SOMETHING ALONG THE LINES OF IS THE CETNER TO COLLISION PERPENDICULAR TO THE WALL?
        {
          set_velocity(kart, ((vec2){0}));
          return true;
        }
        //insert other cases
    }
    return false;
}
