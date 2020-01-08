#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "vec2.h"

char init_sdl();
char init_window();
char init_renderer();

void game_loop();
void render();
void process_input(int type, SDL_Keysym keysym);

SDL_Texture * load_image();

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td);

SDL_Point window_size = {640, 480};
vec2 player_pos = {100, 100};
SDL_Point wasd = {0, 0};
char running = 1;

SDL_Window* window;
SDL_Renderer* renderer;

struct timespec init_time;
struct timespec last_time;

SDL_Texture * bg_image;

int main( int argc, char* args[] )
{
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	if ( !(init_sdl() && init_window() && init_renderer()) ) return -1;

	// SDL_Surface * bg_image_surface = SDL_LoadBMP("test_image.bmp");
	// bg_image = SDL_CreateTextureFromSurface(renderer, bg_image_surface);
	// SDL_free(bg_image_surface);

	bg_image = load_image("test_image.bmp");

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

		render(dt);

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

void render(double dt)
{
	struct timespec current_time;
	clock_gettime(CLOCK_MONOTONIC, &current_time);
	struct timespec run_time_spec;
	sub_timespec(init_time, current_time, &run_time_spec);

	double run_time = run_time_spec.tv_sec + run_time_spec.tv_nsec/1000000000.0;

	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	SDL_RenderClear( renderer );

	SDL_Rect rect2;
	rect2.x = player_pos.x;
	rect2.y = player_pos.y;
	rect2.w = 64*3;
	rect2.h = 48*3;

	//SDL_RenderCopyEx(renderer, bg_image, NULL, &rect2, sin(run_time * 2) * 40, NULL, 0);
	//SDL_RenderCopyEx(renderer, bg_image, &rect2, NULL, sin(run_time * 2) * 40, NULL, 0);

	SDL_RenderCopyEx(renderer, bg_image, &rect2, NULL, 0, NULL, 0);

	// SDL_Rect rect3;
	// rect3.x = 100;
	// rect3.y = -100;
	// rect3.w = 64*10;
	// rect3.h = 48*10;
	// SDL_RenderCopyEx(renderer, bg_image, NULL, &rect3, 0, NULL, 0);

	// SDL_SetRenderDrawColor( renderer, 20, 150, 200, 255 );
	// SDL_Rect rect;
	// rect.x = player_pos.x;
	// rect.y = player_pos.y;
	// rect.w = 40;
	// rect.h = 25;
	// SDL_RenderFillRect( renderer, &rect );

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

SDL_Texture* load_image(char* file_name)
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
