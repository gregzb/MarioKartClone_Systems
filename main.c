#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vec2.h"

char init_sdl();
char init_window();
char init_renderer();

void game_loop();
void render();
void process_input(int type, SDL_Keysym keysym);

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td);

SDL_Point window_size = {640, 480};
vec2 player_pos = {100, 100};
SDL_Point wasd = {0, 0};
char running = 1;

SDL_Window* window;
SDL_Renderer* renderer;

struct timespec init_time;
struct timespec last_time;

int main( int argc, char* args[] )
{
	if ( !(init_sdl() && init_window() && init_renderer()) )
		return -1;

  clock_gettime(CLOCK_MONOTONIC, &init_time);
	last_time = init_time;

	game_loop();
}

void game_loop() {
	while (running) {

		struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    struct timespec delta_timespec;
    sub_timespec(last_time, current_time, &delta_timespec);

		double dt = delta_timespec.tv_sec + delta_timespec.tv_nsec/1000000000.0;

		if (dt < 1/60.0) {
			continue;
		}

		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT ) {
				running = 0;
			} else if ( (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {
				process_input(event.type, event.key.keysym);
			}
		}

		player_pos.x += wasd.x * 200 * dt;
		player_pos.y += wasd.y * 200 * dt;

		render();

		clock_gettime(CLOCK_MONOTONIC, &last_time);
	}
}

void process_input(int type, SDL_Keysym keysym) {
	int multiplier = type == SDL_KEYDOWN ? 1 : -1;
	if (keysym.sym == SDLK_w) {
		wasd.y -= multiplier;
	} else if (keysym.sym == SDLK_s) {
		wasd.y += multiplier;
	} else if (keysym.sym == SDLK_a) {
		wasd.x -= multiplier;
	} else if (keysym.sym == SDLK_d) {
		wasd.x += multiplier;
 }
}

void render()
{
	// Clear the window and make it all green
	SDL_RenderClear( renderer );

	// Change color to blue
	SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

	// Render our "player"
	SDL_Rect rect;
	rect.x = player_pos.x;
	rect.y = player_pos.y;
	rect.w = 40;
	rect.h = 25;

	SDL_RenderFillRect( renderer, &rect );

	// Change color to green
	SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );

	// Render the changes above
	SDL_RenderPresent( renderer);
}

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td) {
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

char init_sdl()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

char init_window()
{
	window = SDL_CreateWindow( "Mario Kart Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size.x, window_size.y, 0 );

	if ( window == NULL )
	{
		printf("Failed to create window: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

char init_renderer()
{
	renderer = SDL_CreateRenderer( window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		return 0;
	}
	SDL_RenderSetLogicalSize( renderer, window_size.x, window_size.y );

	return 1;
}
