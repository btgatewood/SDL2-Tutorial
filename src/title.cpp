#include "main.h"

extern App app;

static void update();
static void render();

SDL_Texture* title_texture;
SDL_Texture* btn_texture;
static int timeout;
int reveal;

void init_title()
{
	app.delegate.update = update;
	app.delegate.render = render;

	for (int& key : app.keyboard)  // clear key states
	{
		key = 0;
	}

	title_texture = load_texture("data/header.png");   // 978x380
	btn_texture = load_texture("data/start_btn.png");  // 410x121

	timeout = FPS * 10;
}

void update()
{
	update_background();

	if (reveal < SCREEN_HEIGHT)
	{
		++reveal;
	}

	if (--timeout <= 0)
	{
		init_highscores();  // switch states
	}

	if (app.keyboard[SDL_SCANCODE_LCTRL] || app.keyboard[SDL_SCANCODE_SPACE] || 
		app.keyboard[SDL_SCANCODE_RCTRL])
	{
		init_scene();
	}
}

void render()
{
	draw_background();
	
	// draw title (& start btn)
	SDL_Rect rect{};
	SDL_QueryTexture(title_texture, nullptr, nullptr, &rect.w, &rect.h);
	rect.h = std::min(rect.h, reveal);
	draw_rect(title_texture, rect, (SCREEN_WIDTH / 2) - (rect.w / 2), 50);

	SDL_QueryTexture(btn_texture, nullptr, nullptr, &rect.w, &rect.h);
	rect.h = std::min(rect.h, reveal);
	draw_rect(btn_texture, rect, (SCREEN_WIDTH / 2) - (rect.w / 2), 450);

	if (timeout % 40 < 20)  // flash text
	{
		draw_text(SCREEN_WIDTH / 2, 650, ALIGN_CENTER, "PRESS FIRE TO PLAY!");
	}
}