#include "main.h"

extern App app;


void on_key_down(SDL_KeyboardEvent* event)
{
	const auto& scancode = event->keysym.scancode;

	if (event->repeat == 0 && scancode < NUM_KEYS && scancode >= 0)
	{
		app.keyboard[scancode] = 1;
	}
}


void on_key_up(SDL_KeyboardEvent* event)
{
	const auto& scancode = event->keysym.scancode;

	if (event->repeat == 0 && scancode < NUM_KEYS && scancode >= 0)
	{
		app.keyboard[scancode] = 0;
	}
}
