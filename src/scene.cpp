#include "main.h"

extern App app;


Scene scene;
Entity player;
SDL_Texture* bullet_texture;


void fire_bullet()
{
	Entity* bullet = new Entity();

	scene.bullets_tail->next = bullet;
	scene.bullets_tail = bullet;

	bullet->texture = bullet_texture;
	// SDL_QueryTexture(bullet_texture, nullptr, nullptr, &b->w, &b->h);
	bullet->w = 32;
	bullet->h = 8;
	bullet->x = player.x;
	bullet->y = player.y;  // center y to player
	bullet->dx = PLAYER_BULLET_SPEED;
	// bullet->dy = 0;
	bullet->health = 1;
	// bullet->reload = 0;
	// bullet->next = nullptr;

	player.reload = 8;  // set reload timer
}

// TODO: Use keyboard for movement, mouse position for aiming!!!
//		 The player's sprite should track the mouse position 
//		 and always be facing the cursor.

void update_player()
{
	player.dx = 0;
	player.dy = 0;

	if (app.keyboard[SDL_SCANCODE_UP] || app.keyboard[SDL_SCANCODE_W])
	{
		player.dy -= PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_DOWN] || app.keyboard[SDL_SCANCODE_S])
	{
		player.dy += PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_LEFT] || app.keyboard[SDL_SCANCODE_A])
	{
		player.dx -= PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_RIGHT] || app.keyboard[SDL_SCANCODE_D])
	{
		player.dx += PLAYER_SPEED;
	}

	player.x += player.dx;
	player.y += player.dy;

	if (player.reload > 0)
	{
		--player.reload;
	}

	if ((app.keyboard[SDL_SCANCODE_LCTRL] || app.keyboard[SDL_SCANCODE_SPACE])
		&& player.reload == 0)
	{
		fire_bullet();
	}
}


void update_bullets()
{
	Entity* prev = &scene.bullets_head;  // store ptr to previous bullet

	for (Entity* b = scene.bullets_head.next; b != nullptr; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (b->x > SCREEN_WIDTH)
		{
			// delete bullet
			if (b == scene.bullets_tail)
			{
				scene.bullets_tail = prev;
			}

			prev->next = b->next;
			delete b;
			b = prev;
		}

		prev = b;
	}
}


void update()
{
	update_player();
	update_bullets();
}


void render()
{
	// TODO: refactor to draw_entity() function

	float x = player.x - (player.w / 2.f);
	float y = player.y - (player.h / 2.f);
	draw(player.texture, x, y, player.w, player.h);

	for (Entity* b = scene.bullets_head.next; b != nullptr; b = b->next)
	{
		float x = b->x - (b->w / 2.f);
		float y = b->y - (b->h / 2.f);
		draw(b->texture, x, y, b->w, b->h);
	}
}


void init_scene()
{
	app.delegate.update = update;
	app.delegate.render = render;

	scene.ships_head.next = &player;
	scene.ships_tail = &player;

	scene.bullets_tail = &scene.bullets_head;

	player.texture = load_texture("data/player.png");   // 1432x1394
	// SDL_QueryTexture(player.texture, nullptr, nullptr, &player.w, &player.w);
	player.w = 64;
	player.h = 64;
	player.x = SCREEN_WIDTH / 4;
	player.y = SCREEN_HEIGHT / 2;

	bullet_texture = load_texture("data/missile.png");  // 517x141
}
