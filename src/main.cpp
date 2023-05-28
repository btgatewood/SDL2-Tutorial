#include "main.h"

App app;

void cap_frame_rate()
{
	static Uint64 prev_time = SDL_GetTicks64();  // could this be causing the bugs?
	static double remainder;

	Uint64 wait = 16 + (int)remainder;	  // accumulator helps keep us closer to 60fps
	remainder -= (int)remainder;		  // casting to int is the trick
	remainder += (1000.0 / 60.0) - 16.0;  // prints "0.666667"

	Uint64 frame_time = SDL_GetTicks64() - prev_time;
	wait -= frame_time;

	if (wait < 1)  // do we need this?
	{
		wait = 1;
	}

	else if (wait > 17)  // prevent freezing (SDL window-related issues?)
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
	init_level();

	while (!app.quit)
	{
		process_events();
		app.delegate.update();
		// SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
		SDL_RenderClear(app.renderer);	  // begin scene
		app.delegate.render();
		SDL_RenderPresent(app.renderer);  // end scene
		cap_frame_rate();				  // TODO: test frame rate
	}

	shutdown();
	return 0;
}