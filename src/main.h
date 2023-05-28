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
const int MAX_MIX_CHANNELS = 8;

const double PI = 3.1415926535897931;
const int PLAYER_SPEED = 6;

enum
{
	WEAPON_PISTOL,
	WEAPON_SHOTGUN,
	WEAPON_SMG,
	NUM_WEAPONS
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
	int ammo[NUM_WEAPONS];
};

//
//	Main ( "App" / "Core" / "Engine" ) Functions
//

// draw
SDL_Texture* load_texture(const char* file);
void draw(SDL_Texture* texture, int x, int y, bool center);
void draw_rotated(SDL_Texture* texture, int x, int y, double angle);

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
void init_level();