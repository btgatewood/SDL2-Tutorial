#include "core.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "scene.h"


App app;
bool quit = false;


void process_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:  // TODO: quit on escape key
			quit = true;
			break;

		case SDL_KEYDOWN:
			on_key_down(&event.key);
			break;
		case SDL_KEYUP:
			on_key_up(&event.key);
			break;
		}
	}
}


int main(int argc, char* argv[])
{
	// TODO: avoid misleading function names (init_scene & begin/end_scene)

	init_SDL();
	init_scene();

	while (!quit)
	{
		process_events();

		app.delegate.update();

		begin_scene();
		app.delegate.render();
		end_scene();

		SDL_Delay(16);
	}

	cleanup();
	return 0;
}
