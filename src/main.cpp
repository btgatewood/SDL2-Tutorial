#include "main.h"

App app;
bool quit = false;
SDL_Texture* crosshair_texture;

void init_SDL()
{
	// TODO: code review for init and create function calls and error messages
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << "\n";
	}

	// TODO: make window & renderer flags config constants and assign to local int flags?
	int window_flags = 0;
	app.window = SDL_CreateWindow("02 Topdown Shooter",
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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		std::cout << "Failed to initialize SDL Mixer.\n";
		// exit(1);
	}
	else
	{
		Mix_AllocateChannels(MAX_MIX_CHANNELS);
	}

	SDL_ShowCursor(0);  // hide the cursor
}

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

		app.textures.push_back(Texture{ file, sdl_texture });
	}

	return sdl_texture;
}

void update() {}

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

void render()
{
	draw(crosshair_texture, app.mouse.x, app.mouse.y, true);
}

void init_scene()
{
	app.delegate.update = update;
	app.delegate.render = render;

	crosshair_texture = load_texture("data/targetter.png");
}

void on_key_down(SDL_KeyboardEvent* event)
{
	const auto& scancode = event->keysym.scancode;

	if (event->repeat == 0 && scancode < NUM_KEYS && scancode >= 0)
	{
		app.keyboard[scancode] = 1;
	}
}

void on_key_up(SDL_KeyboardEvent* event)
{
	const auto& scancode = event->keysym.scancode;

	if (event->repeat == 0 && scancode < NUM_KEYS && scancode >= 0)
	{
		app.keyboard[scancode] = 0;
	}
}

void process_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:  // TODO: quit on escape key
			quit = true;
			break;

		case SDL_KEYDOWN:
			on_key_down(&event.key);
			break;
		case SDL_KEYUP:
			on_key_up(&event.key);
			break;

		case SDL_TEXTINPUT:
			app.input_text = event.text.text;
			break;
		}
	}

	// read the current mouse position
	SDL_GetMouseState(&app.mouse.x, &app.mouse.y);
}

void cap_frame_rate()
{
	static Uint64 prev_time = SDL_GetTicks64();  // could this be causing the bugs?
	static double remainder;

	// TODO: review types, conversions, & casting
	Uint64 wait = 16 + (int)remainder;  // accumulator helps keep us closer to 60fps
	remainder -= (int)remainder;  // casting to int is the trick
	remainder += (1000.0 / 60.0) - 16.0;  // prints "0.666667"

	Uint64 frame_time = SDL_GetTicks64() - prev_time;
	wait -= frame_time;

	if (wait < 1)  // do we need this?
	{
		wait = 1;
	}

	if (wait > 17)  // prevent freezing (SDL window-related issues?)
	{
		wait = 17;
	}

	SDL_Delay(static_cast<Uint32>(wait));
	prev_time = SDL_GetTicks64();
}

void shutdown()
{
	Mix_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	unsigned int seed = static_cast<unsigned int>(time(nullptr));
	srand(seed);
	std::cout << "Seeded rng with {" << seed << "}.\n";

	init_SDL();
	init_scene();

	while (!quit)
	{
		process_events();
		app.delegate.update();
		SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
		SDL_RenderClear(app.renderer);	  // begin scene
		app.delegate.render();
		SDL_RenderPresent(app.renderer);  // end scene
		cap_frame_rate();				  // TODO: test frame rate
	}

	shutdown();
	return 0;
}