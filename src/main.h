#pragma once

#include <SDL.h>
#include <SDL_image.h>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int PLAYER_SPEED = 4;
const int PLAYER_BULLET_SPEED = 16;

const int NUM_KEYS = 256;  // index keyboard using SDL scancodes


// draw
SDL_Texture* load_texture(const char* file);  // TODO: destroy textures?
void begin_scene();
void end_scene();
void draw(SDL_Texture* texture, float x, float y, int w, int h);
void draw(SDL_Texture* texture, int x, int y);
void draw(SDL_Texture* texture, int x, int y, float scale);


// init
void init_SDL();
void cleanup();


// input
void on_key_down(SDL_KeyboardEvent* event);
void on_key_up(SDL_KeyboardEvent* event);


// scene
void init_scene();  // should this be in init.cpp?


// structs
struct Delegate
{
	// TODO: use c++ function pointers? (std::function?)
	void (*update)(void);
	void (*render)(void);
};

struct App
{
	SDL_Renderer* renderer;
	SDL_Window* window;
	Delegate delegate;
	int keyboard[NUM_KEYS];
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
	int reload;
	Entity* next;
};

struct Scene
{
	// TODO: avoid linked lists, use c++ stl
	Entity ships_head;  // container object
	Entity* ships_tail;
	Entity bullets_head;
	Entity* bullets_tail;
};
