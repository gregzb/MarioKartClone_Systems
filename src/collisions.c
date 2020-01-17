
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
#include "collisions.h"


bool check_terrain_collisions(struct kart * cart, SDL_Rect * terrain_rect, SDL_Rect * resultant) {
    if (SDL_IntersectRect(cart->kart_rect, terrain_rect, resultant)){
        kart_reverse_velocity(cart);
        kart_reverse_direction(cart);
        return true;
    }
    return false; 
}








