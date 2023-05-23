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
const int MAX_MIX_CHANNELS = 8;

const double PI = 3.1415926535897931;
const int PLAYER_SPEED = 6;

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
	int angle;
};

struct Scene
{
	std::vector<Entity> entities;
};

// main ( app / core / engine )
SDL_Texture* load_texture(const char* file);
void draw(SDL_Texture* texture, int x, int y, bool center);

// game
void init_level();