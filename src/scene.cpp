// TODO: Use keyboard for movement, mouse position for aiming!!!
//		 The player's sprite should track the mouse position 
//		 and always be facing the cursor.
#include "main.h"

extern App app;


// function declarations
// TODO: move update() and render() to top of module?
void reset_scene();

void fire_bullet();
void fire_alien_bullet(Entity& entity);
void spawn_enemy();
void clamp_player();
bool ship_collision(Entity& bullet);

void update();
void update_player();
void update_enemy_ai();
void update_ships();
void update_bullets();

void render();


// static game objects & data
Scene scene;
Entity player;

SDL_Texture* player_texture;
SDL_Texture* bullet_texture;
SDL_Texture* alien_texture;
SDL_Texture* alien_bullet_texture;

int enemy_spawn_timer;
int scene_reset_timer;


// function definitions
void init_scene()
{
	app.delegate.update = update;
	app.delegate.render = render;

	// NOTE: these vars never change and always point to player, right?
	//		 (...so do we really need them?)
	// scene.ships_head.next = &player; 
	// scene.ships_tail = &player;

	// load textures
	player_texture = load_texture("data/player.png");			  // 1432x1394
	bullet_texture = load_texture("data/bullet_player.png");	  // 517x141
	alien_texture = load_texture("data/alien.png");				  // 768x813
	alien_bullet_texture = load_texture("data/bullet_alien.png"); // 182x35

	reset_scene();
}


void reset_scene()
{
	Entity* e = nullptr;

	while (player.next)  // delete alien ships in linked list
	{
		e = player.next;
		player.next = e->next;
		delete e;
	}

	while (scene.bullets_head.next)
	{
		e = scene.bullets_head.next;
		scene.bullets_head.next = e->next;
		delete e;
	}

	scene.ships_tail = &player;
	scene.bullets_tail = &scene.bullets_head;

	// init player
	player.texture = player_texture;
	player.w = 64;
	player.h = 64;
	player.x = SCREEN_WIDTH / 4;
	player.y = SCREEN_HEIGHT / 2;
	player.health = 3;
	player.owner = PLAYER;

	enemy_spawn_timer = 0;
	scene_reset_timer = FPS * 2;
}


void fire_bullet()
{
	Entity* bullet = new Entity();

	scene.bullets_tail->next = bullet;
	scene.bullets_tail = bullet;

	bullet->texture = bullet_texture;
	bullet->w = 32;
	bullet->h = 8;

	// position the bullet at player's center
	bullet->x = player.x + (player.w / 2.f) - (bullet->w / 2.f);
	bullet->y = player.y + (player.h / 2.f) - (bullet->h / 2.f);

	bullet->dx = PLAYER_BULLET_SPEED;

	bullet->health = 1;
	bullet->owner = PLAYER;

	player.reload = 8;  // set reload timer
}


void fire_alien_bullet(Entity& entity)
{
	Entity* bullet = new Entity();

	scene.bullets_tail->next = bullet;
	scene.bullets_tail = bullet;

	bullet->texture = alien_bullet_texture;
	bullet->w = 32;
	bullet->h = 8;

	bullet->x = entity.x + (entity.w / 2.f) - (bullet->w / 2.f);
	bullet->y = entity.y + (entity.h / 2.f) - (bullet->h / 2.f);

	// TODO: change function params to float?
	calculate_slope(static_cast<int>(player.x) + (player.w / 2), 
					static_cast<int>(player.y) + (player.h / 2), 
					static_cast<int>(bullet->x), 
					static_cast<int>(bullet->y), 
					bullet->dx, bullet->dy);  // sets dx, dy

	bullet->dx *= ALIEN_BULLET_SPEED;
	bullet->dy *= ALIEN_BULLET_SPEED;

	bullet->health = 1;
	bullet->owner = ALIENS;

	entity.reload = FPS * (1 + rand() % 3);  // random reload timer
}


void spawn_enemy()
{
	Entity* enemy = new Entity();

	scene.ships_tail->next = enemy;
	scene.ships_tail = enemy;

	enemy->texture = alien_texture;
	enemy->w = 48;
	enemy->h = 48;
	enemy->x = SCREEN_WIDTH;
	enemy->y = rand() % SCREEN_HEIGHT * 1.f;
	enemy->dx = -(2.f + (rand() % 4));  // random speed
	enemy->health = 1;
	enemy->reload = FPS * (1 + rand() % 3);
	enemy->owner = ALIENS;

	enemy_spawn_timer = 30 + (rand() % FPS * 3);
}


void clamp_player()
{
	if (player.health != 0)
	{
		if (player.x < 0)
		{
			player.x = 0;
		}
		if (player.x > SCREEN_WIDTH - player.w)
		{
			player.x = static_cast<float>(SCREEN_WIDTH - player.w);
		}

		if (player.y < 0)
		{
			player.y = 0;
		}
		if (player.y > SCREEN_HEIGHT - player.h)
		{
			player.y = static_cast<float>(SCREEN_HEIGHT - player.h);
		}
	}
}


bool ship_collision(Entity& bullet)
{
	for (Entity* e = &player; e != nullptr; e = e->next)
	{
		// use mid-right for player bullets, mid-left for alien bullets
		float x = bullet.owner == PLAYER ? bullet.x + (bullet.w * 0.75f) : 
										   bullet.x + (bullet.w * 0.25f);
		float center_y = bullet.y + (bullet.h / 2.f);

		if (e->owner != bullet.owner && collision_point(*e, x, center_y))
		{
			// decrease entity & bullet health
			e->health -= 1;
			bullet.health = 0;
			return true;
		}
	}

	return false;
}


void update()
{
	update_player();
	clamp_player();

	update_enemy_ai();

	update_ships();
	update_bullets();

	if (--enemy_spawn_timer <= 0)
	{
		spawn_enemy();
	}

	if (player.health <= 0)
	{
		--scene_reset_timer;

		if (scene_reset_timer <= 0)
		{
			reset_scene();
		}
	}
}


void update_player()
{
	if (player.health <= 0)
	{
		return;
	}

	player.dx = 0;
	player.dy = 0;

	// move with arrow keys or wasd
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

	// fire with left ctrl, space, or right ctrl
	if (app.keyboard[SDL_SCANCODE_LCTRL] || app.keyboard[SDL_SCANCODE_SPACE] ||
		app.keyboard[SDL_SCANCODE_RCTRL])
	{
		if (player.reload == 0)
		{
			fire_bullet();
		}
	}
}


void update_enemy_ai()
{
	for (Entity* e = player.next; e != nullptr; e = e->next)
	{
		if (--e->reload <= 0)  // decrement timer
		{
			fire_alien_bullet(*e);
		}
	}
}


void update_ships()
{
	Entity* prev = &player;

	for (Entity* e = player.next; e != nullptr; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		// delete enemy if it's dead or offscreen
		if (e->health <= 0 || e->x < -e->w)  
		{
			if (e == scene.ships_tail)
			{
				scene.ships_tail = prev;
			}

			prev->next = e->next;
			delete e;
			e = prev;
		}

		prev = e;
	}
}


bool is_offscreen(const Entity& entity)
{
	return entity.x < -entity.w || entity.x > SCREEN_WIDTH ||
		entity.y < -entity.y || entity.y > SCREEN_HEIGHT;
}


void update_bullets()
{
	Entity* prev = &scene.bullets_head;  // lets us delete entity & re-link list
	for (Entity* b = scene.bullets_head.next; b != nullptr; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (is_offscreen(*b) || ship_collision(*b))
		{
			if (b == scene.bullets_tail)
			{
				scene.bullets_tail = prev;
			}
			prev->next = b->next;
			delete b;  // delete bullet
			b = prev;
		}

		prev = b;
	}
}


void render()
{
	begin_scene();

	// NOTE: we stopped centering the texture to the entity's position here 
	//		 to make collision detection easier.
	// float x = player.x - (player.w / 2.f);
	// float y = player.y - (player.h / 2.f);
	// draw(player.texture, x, y, player.w, player.h);

	draw(player);

	for (Entity* e = player.next; e != nullptr; e = e->next)
	{
		draw(*e);
	}

	for (Entity* b = scene.bullets_head.next; b != nullptr; b = b->next)
	{
		draw(*b);
	}

	end_scene();
}
