#include <SDL2/SDL.h>
#include <stdio.h>

char init_sdl();
char init_window();
char init_renderer();

void game_loop();
void render();
void process_input(int type, SDL_Keysym keysym);

SDL_Point window_size = {640, 480};
SDL_Point player_pos = {100, 100};
SDL_Point wasd = {0, 0};
char running = 1;

SDL_Window* window;
SDL_Renderer* renderer;

int main( int argc, char* args[] )
{
	if ( !(init_sdl() && init_window() && init_renderer()) )
		return -1;

	game_loop();
}

void game_loop() {
	while (running) {
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT ) {
				running = 0;
			} else if ( (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {
				process_input(event.type, event.key.keysym);
			}
		}

		player_pos.x += wasd.x * 10;
		player_pos.y += wasd.y * 10;

		render();

		SDL_Delay(16);
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
