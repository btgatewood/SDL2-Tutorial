#include "main.h"


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


void cap_frame_rate(Uint64& prev_time, float& remainder)
{
	long wait = 16 + remainder;  // accumulator helps keep us closer to 60fps
	remainder -= static_cast<int>(remainder);  // casting to int is the trick
	remainder += 0.667;

	long frame_time = SDL_GetTicks64() - prev_time;
	wait -= frame_time;
	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);
	prev_time = SDL_GetTicks64();
}


int main(int argc, char* argv[])
{
	// TODO: avoid misleading function names (init_scene & begin/end_scene)

	init_SDL();
	init_scene();

	Uint64 prev_time = SDL_GetTicks64();
	float remainder = 0.0f;

	while (!quit)
	{
		process_events();

		app.delegate.update();

		begin_scene();
		app.delegate.render();
		end_scene();

		// TODO: Test frame rate.
		cap_frame_rate(prev_time, remainder);
	}

	cleanup();
	return 0;
}
