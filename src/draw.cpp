#include "main.h"

extern App app;

SDL_Texture* get_texture(const char* file)
{
	for (const Texture& tex : app.textures)
	{
		if (tex.filename == file)
		{
			return tex.texture;
		}
	}

	return nullptr;
}

SDL_Texture* load_texture(const char* file)
{
	SDL_Texture* sdl_texture = get_texture(file);

	if (!sdl_texture)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
			"Loading %s", file);
		sdl_texture = IMG_LoadTexture(app.renderer, file);

		if (!sdl_texture)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
				"Failed to load %s", file);
		}

		app.textures.push_back(Texture{ file, sdl_texture });
	}

	return sdl_texture;
}

void draw(SDL_Texture* texture, int x, int y, bool center)
{
	SDL_Rect dstrect{ x, y, 0, 0 };
	SDL_QueryTexture(texture, nullptr, nullptr, &dstrect.w, &dstrect.h);
	if (center)
	{
		dstrect.x -= dstrect.w / 2;
		dstrect.y -= dstrect.h / 2;
	}
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}

void draw_rect(SDL_Texture* texture, const SDL_Rect& src, int x, int y)
{
	SDL_Rect dest{ x, y, src.w, src.h };
	SDL_RenderCopy(app.renderer, texture, &src, &dest);
}

void draw_rotated(SDL_Texture* texture, int x, int y, double angle)  // auto-centered, uses ext function
{
	SDL_Rect dstrect{ x, y, 0, 0 };
	SDL_QueryTexture(texture, nullptr, nullptr, &dstrect.w, &dstrect.h);
	dstrect.x -= dstrect.w / 2;
	dstrect.y -= dstrect.h / 2;
	SDL_RenderCopyEx(app.renderer, texture, nullptr, &dstrect, angle, nullptr, SDL_FLIP_NONE);
}