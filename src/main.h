#pragma once

#include <iostream>			// using std::cout to print to console
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

const int FPS = 60;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int NUM_KEYS = 256;	// keystates are indexed using SDL scancodes
const int MAX_MIX_CHANNELS = 8;

struct Texture
{
	std::string filename;
	SDL_Texture* texture;
};

struct Delegate				// TODO: use c++ standard function pointers?
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
	SDL_Renderer* renderer;
	SDL_Window* window;

	Delegate delegate;

	int keyboard[NUM_KEYS];
	Mouse mouse;

	std::vector<Texture> textures;
	std::string input_text;
};