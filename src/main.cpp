#include "core.h"
#include "draw.h"
#include "init.h"
#include "input.h"


App app;
Entity player;
Entity bullet;
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
	init_SDL();
	
	player.x = SCREEN_WIDTH / 4;
	player.y = SCREEN_HEIGHT / 2;
	player.texture = load_texture("data/player.png");  // 1432x1394

	bullet.texture = load_texture("data/missile.png");  // 517x141

	while (!quit)
	{
		process_events();

		if (app.up)
		{
			player.y -= 4;
		}
		if (app.down)
		{
			player.y += 4;
		}
		if (app.left)
		{
			player.x -= 4;
		}
		if (app.right)
		{
			player.x += 4;
		}

		if (app.fire && bullet.health == 0)
		{
			bullet.x = player.x;
			bullet.y = player.y;
			bullet.dx = 16;
			bullet.dy = 0;
			bullet.health = 1;
		}

		if (bullet.health > 0)
		{
			bullet.x += bullet.dx;
			bullet.y += bullet.dy;

			if (bullet.x > SCREEN_WIDTH)
			{
				bullet.health = 0;
			}
		}

		begin_scene();

		// set size to 64x64 and center texture to player position
		draw(player.texture, player.x - 32, player.y - 32, 64, 64);

		if (bullet.health > 0)
		{
			draw(bullet.texture, bullet.x - 16, bullet.y - 4, 32, 8);
		}

		end_scene();
		SDL_Delay(16);
	}

	cleanup();
	return 0;
}
