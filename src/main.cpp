#include "main.h"

App app;
bool quit = false;

void process_events()
{
	app.input_text.clear();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:  // TODO: quit on escape key
			quit = true;  // TODO: make quit global and move to input.cpp?
			break;

		case SDL_KEYDOWN:
			on_key_down(&event.key);
			break;
		case SDL_KEYUP:
			on_key_up(&event.key);
			break;

		case SDL_TEXTINPUT:
			app.input_text = event.text.text;
			break;
		}
	}
}

void cap_frame_rate()
{
	static Uint64 prev_time = SDL_GetTicks64();  // could this be causing the bugs?
	static double remainder;

	// TODO: review types, conversions, & casting
	Uint64 wait = 16 + (int)remainder;  // accumulator helps keep us closer to 60fps
	remainder -= (int)remainder;  // casting to int is the trick
	remainder += (1000.0 / 60.0) - 16.0;  // prints "0.666667"

	Uint64 frame_time = SDL_GetTicks64() - prev_time;
	wait -= frame_time;

	if (wait < 1)  // do we need this?
	{
		wait = 1;
	}

	if (wait > 17)  // prevent freezing (SDL window-related issues?)
	{
		wait = 17;
	}

	SDL_Delay(static_cast<Uint32>(wait));
	prev_time = SDL_GetTicks64();
}

int main(int argc, char* argv[])
{
	unsigned int seed = static_cast<unsigned int>(time(nullptr));
	srand(seed);
	std::cout << "Seeded rng with {" << seed << "}.\n";

	init_SDL();
	init_game();
	init_highscores();

	while (!quit)
	{
		process_events();
		app.delegate.update();
		begin_scene();
		app.delegate.render();
		end_scene();
		cap_frame_rate();  // TODO: test frame rate
	}

	cleanup();
	return 0;
}
