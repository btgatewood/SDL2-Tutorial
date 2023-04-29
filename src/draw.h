#pragma once

#include "core.h"


SDL_Texture* load_texture(const char* file);  // TODO: destroy textures?

void begin_scene();
void end_scene();

void draw(SDL_Texture* texture, float x, float y, int w, int h);
void draw(SDL_Texture* texture, int x, int y);
void draw(SDL_Texture* texture, int x, int y, float scale);