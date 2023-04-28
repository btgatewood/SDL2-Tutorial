#include <iostream>
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


App app;
Entity player;
Entity bullet;
bool quit = false;


void init_SDL()
{
	// TODO: code review for init and create function calls and error messages
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

	IMG_Init(IMG_INIT_PNG);  // init png support needs error checking
}


SDL_Texture* load_texture(const char* file)
{
	SDL_Texture* texture;
	// SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
	//	"Loading %s", file);
	texture = IMG_LoadTexture(app.renderer, file);
	return texture;
}


void cleanup()
{
	IMG_Quit();
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}


// TODO: add wasd support
void on_key_down(SDL_KeyboardEvent* event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			app.down = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			app.left = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 1;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_SPACE)
		{
			app.fire = 1;
		}
	}
}


// TODO: add wasd support
void on_key_up(SDL_KeyboardEvent* event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			app.up = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			app.down = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			app.left = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			app.right = 0;
		}
		else if (event->keysym.scancode == SDL_SCANCODE_SPACE)
		{
			app.fire = 0;
		}
	}
}


void process_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			on_key_down(&event.key);
			break;
		case SDL_KEYUP:
			on_key_up(&event.key);
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


void draw(SDL_Texture* texture, int x, int y, int w, int h)
{
	SDL_Rect dstrect{ x, y, w, h };
	SDL_RenderCopy(app.renderer, texture, nullptr, &dstrect);
}


int main(int argc, char* argv[])
{
	init_SDL();
	
	player.x = SCREEN_WIDTH / 8;
	player.y = SCREEN_HEIGHT / 2;
	player.texture = load_texture("data/player.png");  // 1432x1394

	bullet.texture = load_texture("data/missile.png");  // 517x141

	while (!quit)
	{
		process_events();

		if (app.up)
		{
			player.y -= 4;
		}
		if (app.down)
		{
			player.y += 4;
		}
		if (app.left)
		{
			player.x -= 4;
		}
		if (app.right)
		{
			player.x += 4;
		}

		if (app.fire && bullet.health == 0)
		{
			bullet.x = player.x;
			bullet.y = player.y;
			bullet.dx = 16;
			bullet.dy = 0;
			bullet.health = 1;
		}

		if (bullet.health > 0)
		{
			bullet.x += bullet.dx;
			bullet.y += bullet.dy;

			if (bullet.x > SCREEN_WIDTH)
			{
				bullet.health = 0;
			}
		}

		begin_scene();
		// set size to 64x64 and center texture to player position
		draw(player.texture, player.x - 32, player.y - 32, 64, 64);
		if (bullet.health > 0)
		{
			// 32x8, centered
			draw(bullet.texture, bullet.x - 16, bullet.y - 4, 32, 8);
		}
		end_scene();

		SDL_Delay(16);
	}

	cleanup();
	return 0;
}
