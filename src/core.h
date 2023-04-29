#pragma once

#include <SDL.h>
#include <SDL_image.h>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int PLAYER_SPEED = 4;
const int PLAYER_BULLET_SPEED = 16;

const int NUM_KEYS = 256;  // num derived from SDL scancodes


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
	int keyboard[NUM_KEYS];  // can we make this array smaller?
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

// TODO: avoid linked lists, use c++ stl

struct Scene
{
	Entity ships_head;  // container object
	Entity* ships_tail;
	Entity bullets_head;
	Entity* bullets_tail;
};
