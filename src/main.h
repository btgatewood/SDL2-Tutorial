#pragma once

#include <iostream>  // std::cout for printing to console
#include <SDL.h>
#include <SDL_image.h>


// config
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;
const int NUM_KEYS = 256;  // index keyboard using SDL scancodes

const int PLAYER_SPEED = 4;
const int PLAYER_BULLET_SPEED = 16;
const int ALIEN_BULLET_SPEED = 8;

const int PLAYER = 0;
const int ALIENS = 1;


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
	int owner;
	Entity* next;
};

struct Scene
{
	// Entity ships_head;  // refactored, using player object as container
	Entity* ships_tail;
	Entity bullets_head;  // object used as static container
	Entity* bullets_tail;
};


// init
void init_SDL();
void cleanup();

// draw
SDL_Texture* load_texture(const char* file);  // TODO: destroy textures?

void begin_scene();
void end_scene();

void draw(const Entity& entity);
void draw(SDL_Texture* texture, float x, float y, int w, int h);
void draw(SDL_Texture* texture, int x, int y);
void draw(SDL_Texture* texture, int x, int y, float scale);

// input
void on_key_down(SDL_KeyboardEvent* event);
void on_key_up(SDL_KeyboardEvent* event);

// scene
void init_scene();  // should this be in init.cpp?

// physics
bool collision_point(const Entity& e, float x, float y);
bool collision_rect(const Entity& e1, const Entity& e2);

void calculate_slope(int x1, int y1, int x2, int y2, float& dx, float& dy);