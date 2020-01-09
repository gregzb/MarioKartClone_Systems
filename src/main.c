#define _GNU_SOURCE

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

#include "vec2.h"
#include "time_util.h"
#include "server.h"

char init_sdl();
char init_window();
char init_renderer();

void game_loop();
void render();
void process_input(int type, SDL_Keysym keysym);

SDL_Texture * load_image();

SDL_Point window_size = {640, 480};
vec2 player_pos = {100, 100};
SDL_Point wasd = {0, 0};
char running = 1;

SDL_Window *window;
SDL_Renderer *renderer;

struct timespec init_time;
struct timespec last_time;

SDL_Texture * bg_image;

int main(int argc, char *args[])
{
	int child = fork();

	//child process
	if (child == 0) {
		server_main();
		//does there need to be a return here?
		//return 0;
	}

	if (!(init_sdl() && init_window() && init_renderer()))
		return -1;

	bg_image = load_image("resources/images/test_image.bmp");

	clock_gettime(CLOCK_MONOTONIC, &init_time);
	last_time = init_time;

	game_loop();
}

void game_loop() {
	while (running) {
		double dt = get_delta_time(last_time);

		if (dt < 1 / 60.0)
		{
			continue;
		}

		char fps_title[256];
		snprintf(fps_title, 256, "Mario Kart Clone | %.2lf FPS", 1/dt);
		SDL_SetWindowTitle(window, fps_title);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat)
			{
				process_input(event.type, event.key.keysym);
			}
		}

		player_pos.x += wasd.x * 200 * dt;
		player_pos.y += wasd.y * 200 * dt;

		render(dt);

		clock_gettime(CLOCK_MONOTONIC, &last_time);
	}
}

void process_input(int type, SDL_Keysym keysym)
{
	int multiplier = type == SDL_KEYDOWN ? 1 : -1;
	if (keysym.sym == SDLK_w)
	{
		wasd.y -= multiplier;
	}
	else if (keysym.sym == SDLK_s)
	{
		wasd.y += multiplier;
	}
	else if (keysym.sym == SDLK_a)
	{
		wasd.x -= multiplier;
	}
	else if (keysym.sym == SDLK_d)
	{
		wasd.x += multiplier;
	}
}

void render(double dt)
{
	double run_time = get_delta_time(init_time);

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

char init_sdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

char init_window()
{
	window = SDL_CreateWindow("Mario Kart Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size.x, window_size.y, 0);

	if (window == NULL)
	{
		printf("Failed to create window: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

char init_renderer()
{
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		return 0;
	}
	SDL_RenderSetLogicalSize(renderer, window_size.x, window_size.y);

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
