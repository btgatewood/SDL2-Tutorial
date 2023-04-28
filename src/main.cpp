#include <iostream>
#include <SDL.h>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct App
{
	SDL_Renderer* renderer;
	SDL_Window* window;
} app;

void init_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << "\n";
	}

	int window_flags = 0;
	app.window = SDL_CreateWindow("Shooter 01", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,  // window pos (x,y)
		SCREEN_WIDTH, SCREEN_HEIGHT, 
		window_flags);
	if (!app.window)
	{
		std::cout << "Failed to open " << SCREEN_WIDTH << " x " << SCREEN_HEIGHT
			<< "window: " << SDL_GetError() << "\n";
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // what's this?

	int renderer_flags = SDL_RENDERER_ACCELERATED;
	app.renderer = SDL_CreateRenderer(app.window, -1, renderer_flags);
	if (!app.renderer)
	{
		std::cout << "Failed to create renderer: " << SDL_GetError() << "\n";
		// exit(1);
	}
}

void cleanup()
{
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}

void process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			cleanup();
			exit(0);
			break;
		}
	}
}

void begin_scene()
{
	SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
	SDL_RenderClear(app.renderer);
}

void end_scene()
{
	SDL_RenderPresent(app.renderer);
}

int main(int argc, char* argv[])
{
	init_SDL();

	while (true)
	{
		process_events();
		begin_scene();
		end_scene();

		SDL_Delay(16);
	}
}