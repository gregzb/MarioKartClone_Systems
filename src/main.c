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
#include <string.h>

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
char init_text();

void game_loop();
void render_menu(double dt);
void render_game(double dt);
void process_input(int type, SDL_Keysym keysym);

SDL_Point window_size = {1280, 720};
SDL_Point wasd = {0, 0};
SDL_Point mouse_pos = {0, 0};
char mouse_clicked = 0;
char mouse_prev_down = 0;
struct kart karts[4];
int num_karts = 1;
char running = 1;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

struct timespec init_time;
struct timespec last_time;

//SDL_Texture * bg_image;
struct level test_level;
SDL_Texture *ship_tex = NULL;

TTF_Font *font = NULL;

int server_pipe[2];
int server_socket = -1;

char server_ip[16];

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

enum game_state game_state;
enum game_state next_game_state;

int main(int argc, char *args[])
{
	error_check(pipe(server_pipe), "created pipe.");
	int pid = fork();

	error_check(pid, "forking process.");

	//child process
	if (pid == 0)
	{
		server_main(server_pipe[0]);
		return 0;
	}

	if (!(init_sdl() && init_window() && init_renderer() && init_text()))
		return -1;

	// //TEST CODE FOR MULTIPLAYER
	// printf("Enter o for single player mode, h to host a multiplayer game, or m to join one.\n");

	game_state = MENU;
	next_game_state = game_state;

	//bg_image = load_image("resources/images/test4.bmp");
	test_level = level_init(renderer, "resources/levels/testlevel.lvl");
	ship_tex = load_image(renderer, "resources/images/smolship.bmp");

	clock_gettime(CLOCK_MONOTONIC, &init_time);
	last_time = init_time;

	karts[0] = kart_init();
	karts[0].position = (vec2){window_size.x / 2, window_size.y / 2};

	karts[1] = kart_init();
	karts[1].position = (vec2){window_size.x / 2 + 50, window_size.y / 2};

	num_karts = 2;

	game_loop();
}

void game_loop()
{
	//printf("Enter o for single player mode, h to host a multiplayer game, or m to join one.\n");

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

		int bitmask = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

		mouse_clicked = (bitmask & SDL_BUTTON(1) && !mouse_prev_down);
		mouse_prev_down = bitmask & SDL_BUTTON(1) ? 1 : 0;

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

		if (game_state == MENU)
		{
			render_menu(dt);
		}

		if (game_state == CONNECTING)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			SDL_Surface *solid = TTF_RenderText_Solid(font, "Connecting to ", (SDL_Color){255, 255, 255, 255});
			SDL_Texture *text_texture = surface_to_texture(renderer, solid);

			SDL_Rect text_bounds = {window_size.x/2, window_size.y/2, 0, 0};

			SDL_QueryTexture(text_texture, NULL, NULL, &text_bounds.w, &text_bounds.h);
			text_bounds.x -= text_bounds.w/2;
			text_bounds.y -= text_bounds.h/2;

			SDL_RenderCopy(renderer, text_texture, NULL, &text_bounds);
			SDL_RenderPresent(renderer);

			printf("Trying to connect to %s\n", server_ip);
			server_socket = connect_to_server(server_ip);
			printf("got back: %d\n", server_socket);
			if (server_socket != -1)
			{
				next_game_state = MULTIPLAYER;
			} else {
				next_game_state = MENU;
			}
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

		if (game_state == SINGLE_PLAYER)
		{
			kart_move(&karts[0], wasd.y, wasd.x, dt);

			render_game(dt);
		}

		game_state = next_game_state;
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

	if (keysym.sym == SDLK_ESCAPE && type == SDL_KEYDOWN)
	{
		next_game_state = MENU;

		//DARIUS, NEXTWORKING CLEANUP HERE
	}
}

// enum game_state process_choice_input(SDL_Keysym keysym)
// {
// 	bool msg = true;
//
// 	switch (keysym.sym)
// 	{
// 	case SDLK_o:
// 		printf("Selected single player mode.\n");
// 		return SINGLE_PLAYER;
// 	case SDLK_m:
// 		printf("Selected connection mode mode.\n");
// 		return CONNECTING;
// 	case SDLK_h:
// 		write(server_pipe[1], &msg, sizeof msg);
//
// 		return CONNECTING;
// 		printf("Selected hosting mode.\n");
// 	default:
// 		return MENU;
// 	}
// }

void render_menu(double dt)
{
	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
	SDL_RenderClear(renderer);

	SDL_Rect single_rect = {window_size.x / 2, 300, 600, 100};
	single_rect.x -= single_rect.w / 2;
	single_rect.y -= single_rect.h / 2;

	char mouse_over_single = render_button(renderer, &single_rect, (SDL_Color){90, 50, 150, 255}, font, "Singleplayer", &mouse_pos, single_rect.h - 20);
	if (mouse_over_single && mouse_clicked)
	{
		next_game_state = SINGLE_PLAYER;
	}

	SDL_Rect multi_create = {window_size.x / 2, 450, 600, 100};
	multi_create.x -= multi_create.w / 2;
	multi_create.y -= multi_create.h / 2;

	char mouse_over_create = render_button(renderer, &multi_create, (SDL_Color){90, 50, 150, 255}, font, "Create Multiplayer", &mouse_pos, single_rect.h - 20);
	if (mouse_over_create && mouse_clicked)
	{
		bool msg = true;
		write(server_pipe[1], &msg, sizeof msg);
		strncpy(server_ip, "127.0.0.1", 15);
		next_game_state = CONNECTING;
	}

	SDL_Rect multi_join = {window_size.x / 2, 600, 600, 100};
	multi_join.x -= multi_join.w / 2;
	multi_join.y -= multi_join.h / 2;

	char mouse_over_join = render_button(renderer, &multi_join, (SDL_Color){90, 50, 150, 255}, font, "Join Multiplayer", &mouse_pos, single_rect.h - 20);
	if (mouse_over_join && mouse_clicked)
	{
		printf("Server IP: ");
		fflush(stdout);
		fgets(server_ip, 16, stdin);
		if(server_ip[14] == '\n') server_ip[14] = 0;
		next_game_state = CONNECTING;
		SDL_Delay(500);
	}

	SDL_RenderPresent(renderer);
}

void render_game(double dt)
{
	double run_time = get_delta_time(init_time);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Rect dst;
	dst.w = window_size.x * 2;
	dst.h = window_size.y * 2;
	dst.x = -karts[0].position.x * 2 + dst.w / 4;
	dst.y = -karts[0].position.y * 2 + dst.h / 4;

	SDL_Point rot_point;
	rot_point.x = karts[0].position.x * 2;
	rot_point.y = karts[0].position.y * 2;

	double rot_angle = v2_angle(karts[0].direction);

	SDL_RenderCopyEx(renderer, test_level.level_image, NULL, &dst, -(rot_angle * 180 / (M_PI)) - 90, &rot_point, 0);

	SDL_Rect center;
	center.w = 25;
	center.h = 25;
	center.x = window_size.x / 2 - center.w / 2;
	center.y = window_size.y / 2 - center.h / 2;

	SDL_RenderCopyEx(renderer, ship_tex, NULL, &center, -90, NULL, 0);

	for (int i = 1; i < num_karts; i++)
	{
		vec2 subbed = v2_sub(karts[i].position, karts[0].position);
		vec2 mult = v2_mult(subbed, 2);
		vec2 rotted = v2_rotate(mult, -rot_angle - M_PI / 2);
		vec2 added = v2_add(rotted, (vec2){center.x + center.w / 2, center.y + center.w / 2});

		SDL_Rect kart_render_pos;
		kart_render_pos.w = 25;
		kart_render_pos.h = 25;
		kart_render_pos.x = (int)added.x - kart_render_pos.w / 2;
		kart_render_pos.y = (int)added.y - kart_render_pos.h / 2;
		SDL_RenderCopyEx(renderer, ship_tex, NULL, &kart_render_pos, -(rot_angle * 180 / M_PI) + 180, NULL, 0);
	}

	SDL_RenderPresent(renderer);
}

char init_text()
{
	if (TTF_Init() == -1)
	{
		printf("Failed to initialize TTF: %s\n", SDL_GetError());
		return 0;
	}
	font = TTF_OpenFont("resources/fonts/Roboto-Black.ttf", 120);
	if (font == NULL)
	{
		printf("Failed to load font: %s\n", SDL_GetError());
		return 0;
	}
	return 1;
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
