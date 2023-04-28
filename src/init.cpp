#include <iostream>

#include "init.h"

extern App app;


void init_SDL()
{
	// TODO: code review for init and create function calls and error messages
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << "\n";
	}

	// TODO: make window & renderer flags config constants and assign to local int flags?
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

	IMG_Init(IMG_INIT_PNG);  // init png support needs error checking
}


void cleanup()
{
	IMG_Quit();
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}