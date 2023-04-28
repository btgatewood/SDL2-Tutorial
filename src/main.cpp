#include <iostream>
#include <SDL.h>
#include <SDL_image.h>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


struct App
{
	SDL_Renderer* renderer;
	SDL_Window* window;
};


struct Entity
{
	SDL_Texture* texture;  // this is a sprite
	int x;
	int y;
};


App app;
Entity player;


void init_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << "\n";
	}

	// TODO: make window & renderer flags config constants and assign to local int flags?
	int window_flags = 0;
	app.window = SDL_CreateWindow("Shooter 01", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,  // window pos (x,y)
		SCREEN_WIDTH, SCREEN_HEIGHT, 
		window_flags);
	if (!app.window)
	{
		std::cout << "Failed to open " << SCREEN_WIDTH << " x " << SCREEN_HEIGHT
			<< "window: " << SDL_GetError() << "\n";
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // what's this?

	int renderer_flags = SDL_RENDERER_ACCELERATED;
	app.renderer = SDL_CreateRenderer(app.window, -1, renderer_flags);
	if (!app.renderer)
	{
		std::cout << "Failed to create renderer: " << SDL_GetError() << "\n";
		// exit(1);
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);  // png & jpg support
}


void cleanup()
{
	IMG_Quit();

	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	SDL_Quit();
}


void process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			cleanup();
			exit(0);
			break;
		}
	}
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


SDL_Texture* load_texture(const char* file)
{
	SDL_Texture* texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
		"Loading %s", file);
	texture = IMG_LoadTexture(app.renderer, file);

	return texture;
}


void draw(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dstrect;
	dstrect.x = x;
	dstrect.y = y;
	SDL_QueryTexture(texture, nullptr, nullptr, &dstrect.w, &dstrect.h);
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}


void draw(SDL_Texture* texture, int x, int y, int w, int h)
{
	SDL_Rect dstrect{ x, y, w, h };
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}


void draw(SDL_Texture* texture, int x, int y, float scale)
{
	SDL_Rect dstrect;
	SDL_QueryTexture(texture, nullptr, nullptr, &dstrect.w, &dstrect.h);
	// calc width & height based on texture size & scale
	dstrect.w = static_cast<int>(dstrect.w * scale);
	dstrect.h = static_cast<int>(dstrect.h * scale);
	// center texture dstrect to position args
	dstrect.x = x - (dstrect.w / 2);
	dstrect.y = y - (dstrect.h / 2);
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}


int main(int argc, char* argv[])
{
	init_SDL();

	player.x = SCREEN_WIDTH / 2;
	player.y = SCREEN_HEIGHT / 2;
	player.texture = load_texture("data/player.png");  // 1394x1432

	while (true)
	{
		process_events();

		begin_scene();
		// set size to 64x64 and center texture to player position
		draw(player.texture, player.x - 32, player.y - 32, 64, 64);
		end_scene();

		SDL_Delay(16);
	}
}
