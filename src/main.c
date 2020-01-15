#define _GNU_SOURCE

#include <SDL2/SDL.h>
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
#include "kart.h"
#include "level.h"
#include "sdl_utils.h"
#include "networking.h"

char init_sdl();
char init_window();
char init_renderer();

void game_loop();
void render();
void process_input(int type, SDL_Keysym keysym);
enum game_state process_choice_input(SDL_Keysym keysym);

SDL_Texture *load_image();

SDL_Point window_size = {640, 480};
SDL_Point wasd = {0, 0};
struct kart main_kart;
char running = 1;

SDL_Window *window;
SDL_Renderer *renderer;

struct timespec init_time;
struct timespec last_time;

//SDL_Texture * bg_image;
struct level test_level;
SDL_Texture *ship_tex;

int pipe_descriptors[2];

enum game_state
{
	MENU = 0,
	SINGLE_PLAYER,
	//single player only
	PAUSED,
	CONNECTING,
	WAIT_PERIOD,
	MULTIPLAYER
};

int main(int argc, char *args[])
{
	error_check(pipe(pipe_descriptors), "created pipe.");
	int pid = fork();

	error_check(pid, "forking process.");

	//child process
	if (pid == 0)
	{
		server_main(pipe_descriptors[0]);
		return 0;
	}

	if (!(init_sdl() && init_window() && init_renderer()))
		return -1;

	//TEST CODE FOR MULTIPLAYER
	printf("Enter o for single player mode, h to host a multiplayer game, or m to join one.\n");

	//bg_image = load_image("resources/images/test4.bmp");
	test_level = level_init(renderer, "resources/levels/testlevel.lvl");
	ship_tex = load_image(renderer, "resources/images/smolship.bmp");

	clock_gettime(CLOCK_MONOTONIC, &init_time);
	last_time = init_time;

	main_kart = kart_init();
	main_kart.position = (vec2){window_size.x / 2, window_size.y / 2};

	game_loop();
}

void game_loop()
{
	printf("Enter o for single player mode, h to host a multiplayer game, or m to join one.\n");

	enum game_state game_state = MENU;
	int server_socket = -1;

	while (running)
	{
		double dt = get_delta_time(last_time);

		if (dt < 1 / 60.0)
		{
			continue;
		}

		char fps_title[256];
		snprintf(fps_title, 256, "Mario Kart Clone | %.2lf FPS", 1 / dt);
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
				if (!game_state)
				{
					game_state = process_choice_input(event.key.keysym);
				}
				else
					process_input(event.type, event.key.keysym);
			}
		}

		if (game_state == CONNECTING)
		{
			server_socket = connect_to_server("127.0.0.1");
			if (server_socket != -1)
			{
				game_state = MULTIPLAYER;
			}
			continue;
		}

		if (game_state == MULTIPLAYER)
		{
			struct server_packet serv_msg;

			//get all queued messages
			while (true)
			{
				ssize_t size = read(server_socket, &serv_msg, sizeof serv_msg);

				if (size == 0)
				{
					//handle gracefully
					printf("Connection to server closed unexpectedly.\n");
					break;
				}
				else if (size < 0)
				{
					if (errno != EWOULDBLOCK && errno != EAGAIN)
					{
						printf("Error reading from server socket: %s\n", strerror(errno));

						//TODO: handle gracefully
						exit(1);
					}
					break;
				}

				switch (serv_msg.type)
				{
				case WAIT_STATUS:
					printf("%d seconds left until game start.\n", serv_msg.data.wait_status.seconds_left);
					break;
				}

			}

			//TODO: send data based on current state
			//This is correct for wait mode
			struct client_packet keep_alive = {.type = KEEP_ALIVE};
			write(server_socket, &keep_alive, sizeof keep_alive);
		}

		if (game_state == SINGLE_PLAYER || game_state == MULTIPLAYER)
		{
			kart_move(&main_kart, wasd.y, wasd.x, dt);

			// player_pos.x += wasd.x * 200 * dt;
			// player_pos.y += wasd.y * 200 * dt;

			render(dt);
		}

		clock_gettime(CLOCK_MONOTONIC, &last_time);
	}
}

void process_input(int type, SDL_Keysym keysym)
{
	int multiplier = type == SDL_KEYDOWN ? 1 : -1;
	if (keysym.sym == SDLK_w)
	{
		wasd.y += multiplier;
	}
	else if (keysym.sym == SDLK_s)
	{
		wasd.y -= multiplier;
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

enum game_state process_choice_input(SDL_Keysym keysym)
{
	bool msg = true;

	switch (keysym.sym)
	{
	case SDLK_o:
		printf("Selected single player mode.\n");
		return SINGLE_PLAYER;
	case SDLK_m:
		printf("Selected connection mode mode.\n");
		return CONNECTING;
	case SDLK_h:
		write(pipe_descriptors[1], &msg, sizeof msg);

		return CONNECTING;
		printf("Selected hosting mode.\n");
	default:
		return MENU;
	}
}

void render(double dt)
{
	double run_time = get_delta_time(init_time);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Rect dst;
	dst.w = window_size.x * 2;
	dst.h = window_size.y * 2;
	dst.x = -main_kart.position.x * 2 + dst.w / 4;
	dst.y = -main_kart.position.y * 2 + dst.h / 4;

	SDL_Point rot_point;
	rot_point.x = main_kart.position.x * 2;
	rot_point.y = main_kart.position.y * 2;

	SDL_RenderCopyEx(renderer, test_level.level_image, NULL, &dst, -v2_angle(main_kart.direction) * 180 / (M_PI)-90, &rot_point, 0);

	SDL_Rect center;
	center.x = window_size.x / 2 - 25 / 2;
	center.y = window_size.y / 2 - 25 / 2;
	center.w = 25;
	center.h = 25;

	SDL_RenderCopyEx(renderer, ship_tex, NULL, &center, -90, NULL, 0);

	SDL_RenderPresent(renderer);
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
