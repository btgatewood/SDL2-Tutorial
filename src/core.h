#pragma once

#include <SDL.h>
#include <SDL_image.h>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


struct App
{
	SDL_Renderer* renderer;
	SDL_Window* window;
	int up;
	int down;
	int left;
	int right;
	int fire;
};


struct Entity
{
	SDL_Texture* texture;  // this is a sprite
	int x;
	int y;
	int dx;
	int dy;
	int health;
};
