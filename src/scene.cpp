// TODO: Use keyboard for movement, mouse position for aiming!!!
//		 The player's sprite should track the mouse position 
//		 and always be facing the cursor.
//
// TODO: Move all calls to rand to separate code;
//		 or create custom utility functions to get random values.
// GOAL: Allow easy configuration of random game settings.
//
// TODO: Add error checking & exception handling.
//
#include "main.h"

extern App app;


// function declarations
void reset_scene();

void fire_bullet();
void fire_alien_bullet(Entity& entity);
void spawn_enemy();
void spawn_debris(const Entity& entity);
void spawn_explosions(int x, int y, int num);
void clamp_player();
bool is_ship_collision(Entity& bullet);

void update();
void update_player();
void update_enemy_ai();
void update_ships();
void update_bullets();
void update_debris();
void update_explosions();

void render();


// static game objects & data
Scene scene;
Entity player;

SDL_Texture* background;
int background_x;

SDL_Texture* player_texture;
SDL_Texture* bullet_texture;
SDL_Texture* alien_texture;
SDL_Texture* alien_bullet_texture;
SDL_Texture* explosion_texture;

int enemy_spawn_timer;
int scene_reset_timer;


// function definitions
void init_scene()
{
	app.delegate.update = update;
	app.delegate.render = render;

	// load textures
	background = load_texture("data/background.png");  // 1920 x 1080
	background_x = 0;

	player_texture = load_texture("data/player.png");			  // 1432x1394
	bullet_texture = load_texture("data/bullet_player.png");	  // 517x141
	alien_texture = load_texture("data/alien.png");				  // 768x813
	alien_bullet_texture = load_texture("data/bullet_alien.png"); // 182x35
	explosion_texture = load_texture("data/explosion.png");		  // 96x96

	reset_scene();
}


void reset_scene()
{
	scene.ships.clear();
	scene.bullets.clear();
	scene.debris_list.clear();
	scene.explosions.clear();

	// init player
	player.texture = player_texture;
	player.w = 128;
	player.h = 128;
	player.x = SCREEN_WIDTH / 4.f;
	player.y = SCREEN_HEIGHT / 2.f;
	player.health = 3;
	player.owner = PLAYER;

	enemy_spawn_timer = 0;
	scene_reset_timer = FPS * 3;
}


void fire_bullet()
{
	Entity bullet{};

	bullet.texture = bullet_texture;
	bullet.w = 64;
	bullet.h = 16;

	// position the bullet at player's center
	bullet.x = player.x + (player.w / 2.f) - (bullet.w / 2.f);
	bullet.y = player.y + (player.h / 2.f) - (bullet.h / 2.f);

	bullet.dx = PLAYER_BULLET_SPEED;

	bullet.health = 1;
	bullet.owner = PLAYER;

	scene.bullets.push_back(std::move(bullet));

	player.reload = 8;  // set reload timer
}


void fire_alien_bullet(Entity& entity)
{
	Entity bullet{};

	bullet.texture = alien_bullet_texture;
	bullet.w = 64;
	bullet.h = 16;

	bullet.x = entity.x + (entity.w / 2.f) - (bullet.w / 2.f);
	bullet.y = entity.y + (entity.h / 2.f) - (bullet.h / 2.f);

	// TODO: change function params to float?
	calculate_slope(static_cast<int>(player.x) + (player.w / 2), 
					static_cast<int>(player.y) + (player.h / 2), 
					static_cast<int>(bullet.x), 
					static_cast<int>(bullet.y), 
					bullet.dx, bullet.dy);  // sets dx, dy

	bullet.dx *= ALIEN_BULLET_SPEED;
	bullet.dy *= ALIEN_BULLET_SPEED;

	bullet.health = 1;
	bullet.owner = ALIENS;

	scene.bullets.push_back(std::move(bullet));

	entity.reload = FPS * (1 + rand() % 3);  // random reload timer
}


void spawn_enemy()
{
	Entity enemy{};

	enemy.texture = alien_texture;
	enemy.w = 96;
	enemy.h = 96;
	enemy.x = SCREEN_WIDTH;
	enemy.y = static_cast<float>(rand() % (SCREEN_HEIGHT - enemy.h));
	enemy.dx = -(2.f + (rand() % 4));  // random speed
	enemy.health = 1;
	enemy.reload = FPS * (1 + rand() % 3);
	enemy.owner = ALIENS;

	scene.ships.push_back(std::move(enemy));

	enemy_spawn_timer = 30 + (rand() % FPS * 1);
}


void spawn_debris(const Entity& entity)
{
	// get texture half width & half height
	int w = 0;
	int h = 0;
	SDL_QueryTexture(entity.texture, nullptr, nullptr, &w, &h);
	w /= 2;
	h /= 2;

	for (int y = 0; y <= h; y += h)  // why?!  // OK, got it.
	{
		for (int x = 0; x <= w; x += w)  // again, why?!
		{
			Debris debris{};

			debris.texture = entity.texture;
			// calc texture src rect (this is why)
			debris.srcrect.x = x;  // x, then w (texture.w / 2)
			debris.srcrect.y = y;  // y, then h (texture.h / 2)
			debris.srcrect.w = w;
			debris.srcrect.h = h;

			debris.x = entity.x + (entity.w / 2.f);
			debris.y = entity.y + (entity.h / 2.f);
			debris.w = entity.w / 2;
			debris.h = entity.h / 2;

			debris.dx = static_cast<float>(rand() % 5) - (rand() % 5);  // that's clever
			debris.dy = static_cast<float>(-(5 + rand() % 12));
			debris.life = FPS * 2;

			scene.debris_list.push_back(std::move(debris));
		}
	}
}


void spawn_explosions(int x, int y, int num)
{
	for (int i = 0; i < num; ++i)
	{
		Explosion exp{};

		exp.x = static_cast<float>(x + (rand() % 32) - (rand() % 32));
		exp.y = static_cast<float>(y + (rand() % 32) - (rand() % 32));

		exp.dx = (rand() % 10) - (rand() % 10) / 10.f;
		exp.dy = (rand() % 10) - (rand() % 10) / 10.f;

		switch (rand() % 4)
		{
		case 0:
			exp.r = 255;
			break;
		case 1:
			exp.r = 255;
			exp.g = 128;
			break;
		case 2:
			exp.r = 255;
			exp.g = 255;
			break;
		default:
			exp.r = 255;
			exp.g = 255;
			exp.b = 255;
			break;
		}

		exp.alpha = rand() % FPS * 3;

		scene.explosions.push_back(std::move(exp));
	}
}


void clamp_player()
{
	if (player.health != 0)
	{
		if (player.x < 0)
		{
			player.x = 0;
		}
		if (player.x > SCREEN_WIDTH / 2.f - player.w / 2.f)
		{
			player.x = static_cast<float>(SCREEN_WIDTH / 2.f - player.w / 2.f);
		}

		if (player.y < 0.f)
		{
			player.y = 0.f;
		}
		if (player.y > SCREEN_HEIGHT - player.h)
		{
			player.y = static_cast<float>(SCREEN_HEIGHT - player.h);
		}
	}
}


void on_ship_collision(Entity& bullet, Entity& ship)
{
	bullet.health = 0;
	ship.health -= 1;

	if (ship.health == 0)
	{
		spawn_debris(ship);

		int x = static_cast<int>(ship.x + (ship.w / 2.f));
		int y = static_cast<int>(ship.y + (ship.h / 2.f));
		int num = 16;  // why?
		spawn_explosions(x, y, num);
	}
}


bool is_ship_collision(Entity& bullet)
{
	// check for collision with player
	float x = bullet.x + (bullet.w * 0.75f);  // mid-right for player bullets
	float center_y = bullet.y + (bullet.h / 2.f);

	if (bullet.owner != PLAYER && collision_point(player, x, center_y))
	{
		on_ship_collision(bullet, player);
		return true;
	}

	// check for collision with aliens
	for (Entity& ship : scene.ships)
	{
		float x = bullet.x + (bullet.w * 0.25f);  // mid-left for alien bullets
		float center_y = bullet.y + (bullet.h / 2.f);

		if (bullet.owner != ALIENS && collision_point(ship, x, center_y))
		{
			on_ship_collision(bullet, ship);
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

	// update background
	if (--background_x < -SCREEN_WIDTH)
	{
		background_x = 0;
	}

	update_ships();
	update_bullets();
	update_debris();
	update_explosions();

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
	for (int i = 0; i < scene.ships.size(); ++i)
	{
		if (--scene.ships[i].reload <= 0)  // decrement reload timer
		{
			fire_alien_bullet(scene.ships[i]);
		}
	}

	//for (Entity& enemy : scene.ships)  // BUG?!  // Nope.
	//{
	//	if (--enemy.reload <= 0)  // decrement reload timer
	//	{
	//		fire_alien_bullet(enemy);
	//	}
	//}
}


void update_ships()
{
	for (auto itr = scene.ships.begin(); itr != scene.ships.end();)
	{
		itr->x += itr->dx;
		itr->y += itr->dy;

		// remove enemy from list if it's dead or offscreen
		if (itr->health <= 0 || itr->x < -itr->w)  
		{
			itr = scene.ships.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


bool is_offscreen(const Entity& entity)  // secret utility function
{
	return (entity.x < -entity.w || entity.x > SCREEN_WIDTH || 
			entity.y < -entity.y || entity.y > SCREEN_HEIGHT);
}


void update_bullets()
{
	for (auto itr = scene.bullets.begin(); itr != scene.bullets.end();)
	{
		itr->x += itr->dx;
		itr->y += itr->dy;

		if (is_offscreen(*itr) || is_ship_collision(*itr))
		{
			itr = scene.bullets.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


void update_debris()
{
	// TODO: Look up removing from vector in game programming c++ book.
	for (auto itr = scene.debris_list.begin(); itr != scene.debris_list.end();)
	{
		itr->x += itr->dx;
		itr->y += itr->dy;

		itr->dy += 0.5;

		if (--itr->life <= 0)
		{
			itr = scene.debris_list.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


void update_explosions()
{
	for (auto itr = scene.explosions.begin();
		itr != scene.explosions.end();
		)
	{
		itr->x += itr->dx;
		itr->y += itr->dy;

		itr->alpha -= 0;

		if (itr->alpha <= 0)  // decrement alpha
		{
			itr = scene.explosions.erase(itr);
		}
		else
		{
			++itr;
		}
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


void draw_explosions()
{
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);

	SDL_SetTextureBlendMode(explosion_texture, SDL_BLENDMODE_ADD);
	for (Explosion& exp : scene.explosions)
	{
		SDL_SetTextureColorMod(explosion_texture, exp.r, exp.g, exp.b);
		SDL_SetTextureAlphaMod(explosion_texture, exp.alpha);

		draw(explosion_texture, exp.x, exp.y);
	}

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}


void render()
{
	begin_scene();

	draw_background();

	if (player.health > 0)
	{
		draw(player);
	}

	for (const Entity& bullet : scene.bullets)
	{
		draw(bullet);
	}

	for (const Entity& ship : scene.ships)  // draws player
	{
		draw(ship);
	}

	for (const Debris& d : scene.debris_list)
	{
		draw(d);
	}

	draw_explosions();

	end_scene();
}
