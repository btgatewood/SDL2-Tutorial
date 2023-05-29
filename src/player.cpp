#include "main.h"

extern App app;

extern Scene scene;
extern Entity player;

void init_player()
{
	player.texture = load_texture("data/donk.png");
	player.x = SCREEN_WIDTH / 2.f;
	player.y = SCREEN_HEIGHT / 2.f;
	SDL_QueryTexture(player.texture, nullptr, nullptr, &player.w, &player.h);

	player.health = 5;

	scene.ammo[WPN_PISTOL] = 12;
	scene.ammo[WPN_SMG] = 100;
	scene.ammo[WPN_SHOTGUN] = 8;
}

void update_player()
{
	// decelerate player, making movement smoother
	player.dx *= 0.85f;
	player.dy *= 0.85f;

	if (app.keyboard[SDL_SCANCODE_UP] || app.keyboard[SDL_SCANCODE_W])
	{
		player.dy = -PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_DOWN] || app.keyboard[SDL_SCANCODE_S])
	{
		player.dy = PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_LEFT] || app.keyboard[SDL_SCANCODE_A])
	{
		player.dx = -PLAYER_SPEED;
	}
	if (app.keyboard[SDL_SCANCODE_RIGHT] || app.keyboard[SDL_SCANCODE_D])
	{
		player.dx = PLAYER_SPEED;
	}

	player.angle = get_angle(static_cast<int>(player.x), static_cast<int>(player.y),
		app.mouse.x, app.mouse.y);

	if (player.reload_timer == 0 && scene.ammo[player.weapon_type] > 0 &&
		app.mouse.button[SDL_BUTTON_LEFT])
	{
		fire_player_bullet();
		--scene.ammo[player.weapon_type];
	}

	// switch weapons
	if (app.mouse.wheel < 0)
	{
		if (--player.weapon_type < WPN_PISTOL)
		{
			player.weapon_type = NUM_WEAPONS - 1;
		}
		app.mouse.wheel = 0;
	}
	else if (app.mouse.wheel > 0)
	{
		if (++player.weapon_type >= NUM_WEAPONS)
		{
			player.weapon_type = WPN_PISTOL;
		}
		app.mouse.wheel = 0;
	}

	// reload pistol
	if (app.mouse.button[SDL_BUTTON_RIGHT])
	{
		if (player.weapon_type == WPN_PISTOL &&
			scene.ammo[WPN_PISTOL] == 0)
		{
			scene.ammo[WPN_PISTOL] = 12;
		}
		app.mouse.button[SDL_BUTTON_RIGHT] = false;
	}
}
