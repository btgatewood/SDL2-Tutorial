#pragma once

#include <iostream>  // using std::cout to print to console
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

const int FPS = 60;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int NUM_KEYS = 256;  // keystates are indexed using SDL scancodes
const int NUM_MOUSE_BUTTONS = 6;

const int MAX_LINE_LENGTH = 1024;
const int GLYPH_WIDTH = 18;
const int GLYPH_HEIGHT = 28;

const int MAX_MIX_CHANNELS = 8;

const double PI = 3.1415926535897931;
const int PLAYER_SPEED = 6;
const int GRID_SIZE = 64;  // background tile size

enum Weapon
{
	WPN_PISTOL,
	WPN_SMG,
	WPN_SHOTGUN,
	NUM_WEAPONS
};

enum Text
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

struct Texture
{
	std::string filename;
	SDL_Texture* texture;
};

struct Delegate  // TODO: use c++ standard function pointers?
{
	void (*update)(void);
	void (*render)(void);
};

struct Mouse
{
	int x;
	int y;
	bool button[NUM_MOUSE_BUTTONS];  // if pressed true, else false
	int wheel;
};

struct App
{
	// graphics
	SDL_Renderer* renderer;
	SDL_Window* window;
	// state
	Delegate delegate;
	bool quit{ false };
	// input
	Mouse mouse;
	int keyboard[NUM_KEYS];
	// resources
	std::vector<Texture> textures;
};

struct Entity
{
	SDL_Texture* texture;
	float x;
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int health;
	int reload_timer;
	int weapon_type;
	double angle;
};

struct Scene
{
	std::vector<Entity> entities;
	// Entity player;
	// SDL_Texture* crosshair_texture;
	// SDL_Texture* bullet_texture;
	int ammo[NUM_WEAPONS]{ 0 };
};

//
//	Main ( "App" / "Core" / "Engine" ) Functions
//

// draw
SDL_Texture* load_texture(const char* file);
void draw(SDL_Texture* texture, int x, int y, bool center);
void draw_rect(SDL_Texture* texture, const SDL_Rect& src, int x, int y);
void draw_rotated(SDL_Texture* texture, int x, int y, double angle);

void draw_text(int x, int y, int align, const std::string& str,
			   int r, int g, int b);

// init
void init_SDL();
void shutdown();

// input
void process_events();

// math/physics/util
inline void calc_slope(int x1, int y1, int x2, int y2, float& dx, float& dy)
{
	int steps = std::max(std::abs(x1 - x2), std::abs(y1 - y2));

	if (steps != 0)  // avoid division by zero
	{
		dx = (x1 - x2) / static_cast<float>(steps);
		dy = (y1 - y2) / static_cast<float>(steps);
	}
	else  // x1,y1 == x2,y2
	{
		dx = 0;
		dy = 0;
	}
}

inline double get_angle(int x1, int y1, int x2, int y2)
{
	double angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / PI);
	return angle >= 0 ? angle : 360 + angle;
}

//
//	Game Functions
//

// scene
void init_scene();
void fire_player_weapon();

// player
void init_player();
void update_player();