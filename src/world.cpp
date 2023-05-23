#include "main.h"

extern App app;

SDL_Texture* crosshair_texture;

void update() {}

void render()
{
	draw(crosshair_texture, app.mouse.x, app.mouse.y, true);
}

void init_level()
{
	app.delegate.update = update;
	app.delegate.render = render;

	crosshair_texture = load_texture("data/targetter.png");
}