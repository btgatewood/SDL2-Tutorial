#include "input.h"

extern App app;


// TODO: add wasd support
void on_key_down(SDL_KeyboardEvent* event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			app.down = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			app.left = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_SPACE)
		{
			app.fire = 1;
		}
	}
}


// TODO: add wasd support
void on_key_up(SDL_KeyboardEvent* event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			app.down = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			app.left = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_SPACE)
		{
			app.fire = 0;
		}
	}
}
