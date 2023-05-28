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

void on_mouse_button_down(SDL_MouseButtonEvent* event)
{
	app.mouse.button[event->button] = true;
}

void on_mouse_button_up(SDL_MouseButtonEvent* event)
{
	app.mouse.button[event->button] = false;
}

void process_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:  // TODO: quit on escape key
			app.quit = true;
			break;

		// keyboard input
		case SDL_KEYDOWN:
			on_key_down(&event.key);
			break;
		case SDL_KEYUP:
			on_key_up(&event.key);
			break;

		// mouse input
		case SDL_MOUSEBUTTONDOWN:
			on_mouse_button_down(&event.button);
			break;
		case SDL_MOUSEBUTTONUP:
			on_mouse_button_up(&event.button);
			break;
		case SDL_MOUSEWHEEL:
			app.mouse.wheel = event.wheel.y; // if up: y > 0, if down: y < 0
			break;

		default:
			break;
		}
	}

	// read the current mouse position
	SDL_GetMouseState(&app.mouse.x, &app.mouse.y);
}