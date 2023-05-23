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

void add_player_bullet();
void add_alien_bullet(Entity& entity);
void add_enemy();
void add_debris(const Entity& entity);
void add_explosions(int x, int y, int num);
void add_score_powerup(float x, float y);

void clamp_player();
bool is_ship_collision(Entity& bullet);
void on_ship_collision(Entity& bullet, Entity& ship);

void update();
void update_player();
void update_enemy_ai();
void update_enemies();
void update_bullets();
void update_debris();
void update_explosions();
void update_powerups();

void render();
void render_text();


// static game objects & data
Scene scene;
Entity player;

SDL_Texture* player_texture;
SDL_Texture* bullet_texture;
SDL_Texture* alien_texture;
SDL_Texture* alien_bullet_texture;
SDL_Texture* explosion_texture;
SDL_Texture* powerup_texture;

int enemy_spawn_timer;
int scene_reset_timer;
int high_score;


// function definitions
void init_scene()
{
	app.delegate.update = update;
	app.delegate.render = render;

	// load textures
	player_texture = load_texture("data/player.png");			  // 1432x1394
	bullet_texture = load_texture("data/bullet_player.png");	  // 517x141
	alien_texture = load_texture("data/alien.png");				  // 768x813
	alien_bullet_texture = load_texture("data/bullet_alien.png"); // 182x35
	explosion_texture = load_texture("data/explosion.png");		  // 96x96
	powerup_texture = load_texture("data/bonus.png");			  // 300x300

	reset_scene();
}


void reset_scene()
{
	scene.enemies.clear();
	scene.bullets.clear();
	scene.debris_list.clear();
	scene.explosions.clear();
	scene.powerups.clear();

	scene.score = 0;

	// init player
	player.texture = player_texture;
	player.w = 96;
	player.h = 96;
	player.x = SCREEN_WIDTH / 4.f;
	player.y = SCREEN_HEIGHT / 2.f;
	player.health = 3;
	player.owner = PLAYER;

	enemy_spawn_timer = 0;
	scene_reset_timer = FPS * 3;
}


void add_player_bullet()
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


void add_alien_bullet(Entity& entity)
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


void add_enemy()
{
	Entity enemy{};

	enemy.texture = alien_texture;
	enemy.w = 96;
	enemy.h = 96;
	enemy.x = SCREEN_WIDTH;
	enemy.y = static_cast<float>(rand() % (SCREEN_HEIGHT - enemy.h));
	enemy.dx = -(2.f + (rand() % 4));
	enemy.dy = (-100 + rand() % 200) / 100.f;
	enemy.health = 1;
	enemy.reload = FPS * (1 + rand() % 3);
	enemy.owner = ALIENS;

	scene.enemies.push_back(std::move(enemy));

	enemy_spawn_timer = 30 + (rand() % FPS * 1);
}


void add_debris(const Entity& entity)
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


void add_explosions(int x, int y, int num)
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


void add_score_powerup(float x, float y)
{
	Entity powerup{};
	powerup.texture = powerup_texture;
	powerup.x = x;
	powerup.y = y;
	powerup.w = 48;
	powerup.h = 48;
	powerup.dx = static_cast<float>(-(rand() % 5));  // can be zero
	powerup.dy = static_cast<float>((rand() % 5) - (rand() % 5));
	powerup.health = FPS * 8;  // 8 sec lifespan
	scene.powerups.push_back(std::move(powerup));
}


void clamp_player()
{
	if (player.health != 0)
	{
		if (player.x < 0)
		{
			player.x = 0;
		}
		if (player.x > SCREEN_WIDTH * 0.75f - player.w / 2.f)
		{
			player.x = SCREEN_WIDTH * 0.75f - player.w / 2.f;
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
		add_debris(ship);

		int x = static_cast<int>(ship.x + (ship.w / 2.f));
		int y = static_cast<int>(ship.y + (ship.h / 2.f));
		int num = 16;  // why?
		add_explosions(x, y, num);

		if (&ship == &player)
		{
			play_sound(SND_PLAYER_DEATH, CH_PLAYER);
		}
		else
		{
			float x = ship.x - (ship.w / 2.f);
			float y = ship.y - (ship.h / 2.f);
			add_score_powerup(x, y);

			play_sound(SND_ALIEN_DEATH, CH_ANY);
		}
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
	for (Entity& ship : scene.enemies)
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

static void update()
{
	update_background();

	update_player();
	clamp_player();

	update_enemy_ai();
	update_enemies();
	update_bullets();
	update_debris();
	update_explosions();
	update_powerups();

	if (--enemy_spawn_timer <= 0)
	{
		add_enemy();
	}

	if (player.health <= 0)
	{
		--scene_reset_timer;

		if (scene_reset_timer <= 0)
		{
			add_highscore(scene.score);
			init_highscores();
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
			add_player_bullet();
			play_sound(SND_PLAYER_FIRE, CH_PLAYER);
		}
	}
}


void update_enemy_ai()
{
	for (Entity& enemy : scene.enemies)
	{
		enemy.y = std::min(std::max(enemy.y, 0.f),  // clamp enemies to screen
						   static_cast<float>(SCREEN_HEIGHT - enemy.h));

		if (--enemy.reload <= 0)  // decrement reload timer
		{
			add_alien_bullet(enemy);
			play_sound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
		}
	}
}


void update_enemies()
{
	for (auto itr = scene.enemies.begin(); itr != scene.enemies.end();)
	{
		itr->x += itr->dx;
		itr->y += itr->dy;

		// remove enemy from list if it's dead or offscreen
		if (itr->health <= 0 || itr->x < -itr->w)  
		{
			itr = scene.enemies.erase(itr);
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
		itr != scene.explosions.end(); )
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


void update_powerups()
{
	for (Entity& powerup : scene.powerups)
	{
		// move
		powerup.x += powerup.dx;
		powerup.y += powerup.dy;

		// clamp (bounce)
		if (powerup.x < 0)
		{
			powerup.x = 0.f;
			powerup.dx = -powerup.dx;
		}
		else if (powerup.x + powerup.w > SCREEN_WIDTH)
		{
			powerup.x = static_cast<float>(SCREEN_WIDTH - powerup.w);
			powerup.dx = -powerup.dx;
		}

		if (powerup.y < 0)
		{
			powerup.y = 0.f;
			powerup.dy = -powerup.dy;
		}
		else if (powerup.y + powerup.h > SCREEN_HEIGHT)
		{
			powerup.y = static_cast<float>(SCREEN_HEIGHT - powerup.h);
			powerup.dy = -powerup.dy;
		}

		// player collision
		if (player.health > 0 && collision_rect(powerup, player))
		{
			powerup.health = 0;
			
			++scene.score;
			high_score = std::max(scene.score, high_score);

			play_sound(SND_POWERUP, CH_POWERUP);
		}

		--powerup.health;  // decrement lifespan timer
	}

	// remove dead powerups 
	auto result = std::remove_if(scene.powerups.begin(), scene.powerups.end(),
		[](Entity& powerup) { return powerup.health <= 0; });  // lambda
	scene.powerups.erase(result, scene.powerups.end());
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


static void render()
{
	draw_background();
	for (const Entity& powerup : scene.powerups)
	{
		// flash powerups with less than 2 seconds left
		if (powerup.health > (FPS * 2) || powerup.health % 12 < 6)
		{
			draw(powerup);
		}
	}
	for (const Entity& bullet : scene.bullets)
	{
		draw(bullet);
	}
	if (player.health > 0)
	{
		draw(player);
	}
	for (const Entity& ship : scene.enemies)  // draws player
	{
		draw(ship);
	}
	for (const Debris& debris : scene.debris_list)
	{
		draw(debris);
	}
	draw_explosions();
	render_text();
}


void render_text()
{
	draw_text(8, 8, ALIGN_LEFT, "SCORE: " + std::to_string(scene.score));

	if (scene.score > 0 && scene.score == high_score)
	{
		draw_text(1024, 8, ALIGN_LEFT, "HIGH SCORE: " + 
				  std::to_string(high_score), 255, 255, 0);  // yellow
	}
	else
	{
		draw_text(1024, 8, ALIGN_LEFT, "HIGH SCORE: " + 
				  std::to_string(high_score));
	}
}
