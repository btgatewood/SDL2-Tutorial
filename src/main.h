#pragma once

#include <iostream>  // std::cout for printing to console
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


// config
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;
const int NUM_KEYS = 256;  // index keyboard using SDL scancodes

const int PLAYER_SPEED = 4;
const int PLAYER_BULLET_SPEED = 16;
const int ALIEN_BULLET_SPEED = 8;

const int PLAYER = 0;  // entity.owner enum
const int ALIENS = 1;

const int MAX_MIX_CHANNELS = 8;

enum Channel
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ALIEN_FIRE
};

enum Sound
{
	SND_PLAYER_FIRE,
	SND_ALIEN_FIRE,
	SND_PLAYER_DEATH,
	SND_ALIEN_DEATH,
	NUM_SOUNDS,
};


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
	// Entity* next;
};

struct Debris
{
	SDL_Texture* texture;
	SDL_Rect srcrect;
	float x;
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int life;
};

struct Explosion
{
	float x;
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int r, g, b;
	int alpha;
};

struct Scene
{
	std::vector<Entity> enemies;
	std::vector<Entity> bullets;
	std::vector<Debris> debris_list;
	std::vector<Explosion> explosions;
};


// init
void init_SDL();
void cleanup();

// draw
SDL_Texture* load_texture(const char* file);  // TODO: destroy textures?

void begin_scene();
void end_scene();

void draw(const Entity& entity);
void draw(const Debris& debris);  // draws a source rect from the texture
void draw(SDL_Texture* texture, float x, float y, int w, int h);
void draw_rect(SDL_Texture* texture, const SDL_Rect& srcrect,
			   float x, float y, int w, int h);
void draw(SDL_Texture* texture, float x, float y);  // uses texture dimensions

// input
void on_key_down(SDL_KeyboardEvent* event);
void on_key_up(SDL_KeyboardEvent* event);

// physics
bool collision_point(const Entity& e, float x, float y);
bool collision_rect(const Entity& e1, const Entity& e2);

void calculate_slope(int x1, int y1, int x2, int y2, float& dx, float& dy);

// scene
void init_scene();  // should this be in init.cpp?

// audio
void load_sounds();
void load_music(const char* file);
void play_music(bool loop);
void play_sound(int id, int channel);