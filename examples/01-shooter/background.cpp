#include "main.h"

extern App app;

SDL_Texture* background;
int background_x;

void init_background()
{
	background = load_texture("data/background.png");  // 1920 x 1080
	background_x = 0;
}

void update_background()
{
	if (--background_x < -SCREEN_WIDTH)
	{
		background_x = 0;
	}
}

void draw_background()
{
	// tiles background texture (infinite scrolling)
	for (int x = background_x; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
	{
		SDL_Rect dstrect{ x,0,SCREEN_WIDTH,SCREEN_HEIGHT };
		SDL_RenderCopy(app.renderer, background, nullptr, &dstrect);
	}
}