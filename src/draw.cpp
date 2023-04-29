#include "draw.h"

extern App app;


SDL_Texture* load_texture(const char* file)
{
	SDL_Texture* texture;
	// SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
	//	"Loading %s", file);
	texture = IMG_LoadTexture(app.renderer, file);
	return texture;
}


void begin_scene()
{
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
	SDL_RenderClear(app.renderer);
}


void end_scene()
{
	SDL_RenderPresent(app.renderer);
}


void draw(SDL_Texture* texture, float x, float y, int w, int h)
{
	SDL_Rect dstrect{ static_cast<int>(x), static_cast<int>(y), w, h };
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}


void draw(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dstrect;
	dstrect.x = x;
	dstrect.y = y;
	SDL_QueryTexture(texture, nullptr, nullptr, &dstrect.w, &dstrect.h);
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}


void draw(SDL_Texture* texture, int x, int y, float scale)
{
	SDL_Rect dstrect;
	SDL_QueryTexture(texture, nullptr, nullptr, &dstrect.w, &dstrect.h);
	// calc width & height based on texture size & scale
	dstrect.w = static_cast<int>(dstrect.w * scale);
	dstrect.h = static_cast<int>(dstrect.h * scale);
	// center texture dest to position args
	dstrect.x = x - (dstrect.w / 2);
	dstrect.y = y - (dstrect.h / 2);
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}
