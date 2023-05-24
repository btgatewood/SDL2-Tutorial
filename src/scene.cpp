#include "main.h"

extern App app;

Scene scene;
Entity player;
SDL_Texture* cursor_texture;

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
}

void update_entities()
{
	// TODO: move player to entities vector? add player reference
	player.x += player.dx;
	player.y += player.dy;

	// clamp player  // TODO: review this math
	player.x = std::min(std::max(player.x, player.w / 2.f), SCREEN_WIDTH - player.w / 2.f);
	player.y = std::min(std::max(player.y, player.h / 2.f), SCREEN_WIDTH - player.h / 2.f);

	for (Entity& e : scene.entities)
	{
		e.x += e.dx;
		e.y += e.dy;
	}
}

void update() 
{
	update_player();
	update_entities();
}

void draw_entities()
{
	for (const Entity& e : scene.entities)
	{
		draw_rotated(e.texture, static_cast<int>(e.x), static_cast<int>(e.y), e.angle);
	}
}

void render()
{
	draw(cursor_texture, app.mouse.x, app.mouse.y, true);
	draw_entities();
	draw_rotated(player.texture, static_cast<int>(player.x), static_cast<int>(player.y), 
				 player.angle);
}

void init_player()
{
	player.texture = load_texture("data/donk.png");
	player.x = SCREEN_WIDTH / 2.f;
	player.y = SCREEN_HEIGHT / 2.f;
	SDL_QueryTexture(player.texture, nullptr, nullptr, &player.w, &player.h);

	player.health = 5;
}

void init_level()
{
	app.delegate.update = update;
	app.delegate.render = render;

	cursor_texture = load_texture("data/targetter.png");

	init_player();
}