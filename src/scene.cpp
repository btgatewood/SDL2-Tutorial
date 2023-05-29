#include "main.h"

extern App app;

Scene scene;
Entity player;
SDL_Texture* cursor_texture;
SDL_Texture* bullet_texture;

void update();
void render();

void init_scene()
{
	app.delegate.update = update;
	app.delegate.render = render;

	cursor_texture = load_texture("data/targetter.png");
	bullet_texture = load_texture("data/bullet.png");

	init_player();
}

void fire_pistol()
{
	Entity bullet{};

	bullet.texture = bullet_texture;
	bullet.x = player.x;
	bullet.y = player.y;
	bullet.health = FPS * 2;
	bullet.angle = player.angle;

	calc_slope(app.mouse.x, app.mouse.y,
		static_cast<int>(bullet.x), static_cast<int>(bullet.y),
		bullet.dx, bullet.dy);

	bullet.dx *= 16;
	bullet.dy *= 16;

	scene.entities.push_back(std::move(bullet));

	player.reload_timer = 16;  // medium reload time
}

void fire_smg()
{
	// NOTE: the only difference from fire_pistol() function is the reload timer
	Entity bullet{};

	bullet.texture = bullet_texture;
	bullet.x = player.x;
	bullet.y = player.y;
	bullet.health = FPS * 2;  // 2 seconds
	bullet.angle = player.angle;  // rotate bullet to match player

	calc_slope(app.mouse.x, app.mouse.y, 
			   static_cast<int>(bullet.x), static_cast<int>(bullet.y), 
			   bullet.dx, bullet.dy);  // sets dx and dy

	bullet.dx *= 16;  // multiplied by 12 in tutorial's text
	bullet.dy *= 16;

	scene.entities.push_back(std::move(bullet));

	player.reload_timer = 4;  // fast reload time
}

void fire_shotgun()
{
	float dx{ 0.f };
	float dy{ 0.f };

	calc_slope(app.mouse.x, app.mouse.y,
		static_cast<int>(player.x), static_cast<int>(player.y),
		dx, dy);

	dx = player.x + (dx * 128);  // project forward from mouse position
	dy = player.y + (dy * 128);

	for (int i = 0; i != 8; ++i)
	{
		Entity bullet{};

		bullet.texture = bullet_texture;
		bullet.x = player.x + rand() % 16 - rand() % 16;  // small random spread for start positions
		bullet.y = player.y + rand() % 16 - rand() % 16;
		bullet.health = FPS * 2;
		bullet.angle = player.angle;

		int dest_x{ static_cast<int>(dx) + rand() % 24 - rand() % 24 };  // med random spread for dest positions
		int dest_y{ static_cast<int>(dy) + rand() % 24 - rand() % 24 };

		calc_slope(dest_x, dest_y, 
			static_cast<int>(bullet.x), static_cast<int>(bullet.y), 
			bullet.dx, bullet.dy);

		bullet.dx *= 16;
		bullet.dy *= 16;

		scene.entities.push_back(std::move(bullet));
	}

	player.reload_timer = static_cast<int>(FPS * 0.75);  // slow reload time
}

void fire_player_bullet()
{
	switch (player.weapon_type)
	{
	case WPN_SMG:
		fire_smg();
		break;

	case WPN_SHOTGUN:
		fire_shotgun();
		break;

	default:
		fire_pistol();
		break;
	}
}

void update_entities()
{
	// TODO: move player to entities vector? add reference to player to scene struct?
	player.x += player.dx;
	player.y += player.dy;

	player.reload_timer = std::max(player.reload_timer - 1, 0);

	// clamp player  // TODO: review this math
	player.x = std::min(std::max(player.x, player.w / 2.f), SCREEN_WIDTH - player.w / 2.f);
	player.y = std::min(std::max(player.y, player.h / 2.f), SCREEN_WIDTH - player.h / 2.f);

	// update bullet positions and remove dead bullets
	for (auto itr = scene.entities.begin(); itr != scene.entities.end(); )
	{
		itr->x += itr->dx;
		itr->y += itr->dy;

		if (--itr->health <= 0)
		{
			itr = scene.entities.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void update() 
{
	update_player();
	update_entities();
}

void draw_weapon_info(const std::string& name, Weapon type, int x, int y)
{
	int r{ 255 };
	int g{ 255 };
	int b{ 255 };

	if (player.weapon_type == type)  // use green text for current weapon type
	{
		r = 0;
		b = 0;
	}

	draw_text(x, y, ALIGN_LEFT, name + ":" + std::to_string(scene.ammo[type]), r, g, b);
}

void render()
{
	// TODO: draw background
	
	// draw entities (bullets)
	for (const Entity& e : scene.entities)
	{
		draw_rotated(e.texture, static_cast<int>(e.x), static_cast<int>(e.y), e.angle);
	}

	// draw player
	draw_rotated(player.texture, 
				 static_cast<int>(player.x), static_cast<int>(player.y), 
				 player.angle);

	// draw hud
	// TODO: research c++ format strings and variable args
	draw_text(10, 10, ALIGN_LEFT, "HEALTH:" + std::to_string(player.health), 255, 255, 255);
	draw_text(250, 10, ALIGN_LEFT, "SCORE:00000", 255, 255, 255);
	draw_weapon_info("PISTOL", WPN_PISTOL, 550, 10);
	draw_weapon_info("SMG", WPN_SMG, 800, 10);
	draw_weapon_info("SHOTGUN", WPN_SHOTGUN, 1050, 10);

	
	// draw cursor
	draw(cursor_texture, app.mouse.x, app.mouse.y, true);
}