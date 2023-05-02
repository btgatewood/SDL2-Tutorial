#include "main.h"

extern App app;


static void update();
static void render();

void handle_name_input();
void draw_name_input();
void draw_highscores();

bool operator<(const HighScore& a, const HighScore& b);
int compare_highscores(const void* a, const void* b);


std::vector<HighScore> highscores;
std::string input_name;
bool is_new_highscore{ false };
int blink_cursor{ 0 };


void init_highscore_table()
{
	for (int i = 0; i < NUM_HIGHSCORES; ++i)
	{
		highscores.push_back(HighScore{ "NULL", NUM_HIGHSCORES - i, 0 });
	}
}

void init_highscores()
{
	app.delegate.update = update;
	app.delegate.render = render;

	for (int& key : app.keyboard)  // clear key states
	{
		key = 0;
	}
}

void add_highscore(int score)
{
	for (HighScore& highscore : highscores)
	{
		highscore.recent = false;
	}

	HighScore new_score{ input_name, score, true };
	highscores.push_back(new_score);  // add recent score

	// sort the recent score into the highscore list
	std::qsort(highscores.data(), highscores.size(), sizeof(HighScore),
		compare_highscores);
	highscores.pop_back();  // remove the lowest score

	for (HighScore& highscore : highscores)
	{
		if (highscore.recent)
		{
			is_new_highscore = true;
		}
	}
}


static void update()
{
	update_background();

	if (is_new_highscore)
	{
		handle_name_input();
	}
	else
	{
		if (app.keyboard[SDL_SCANCODE_SPACE] ||
			app.keyboard[SDL_SCANCODE_LCTRL] ||
			app.keyboard[SDL_SCANCODE_RCTRL])
		{
			init_scene();
		}
	}

	if (++blink_cursor >= FPS)
	{
		blink_cursor = 0;
	}
}

static void render()
{
	draw_background();

	if (is_new_highscore)
	{
		draw_name_input();
	}
	else
	{
		draw_highscores();
	}
}


void handle_name_input()
{
	for (char c : app.input_text)
	{
		c = std::toupper(c);

		if (c >= ' ' && c <= 'Z')  // is char in the range we support?
		{
			input_name += c;  // update name str
		}
	}

	if (app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		input_name.pop_back();
		app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
	}

	if (app.keyboard[SDL_SCANCODE_RETURN])
	{
		if (input_name.length() == 0)
		{
			input_name = "NULL";
		}

		for (HighScore& highscore : highscores)
		{
			if (highscore.recent)
			{
				highscore.name = input_name;
			}
		}

		is_new_highscore = false;
	}
}

void draw_name_input()
{
	draw_text(SCREEN_WIDTH / 2, 75, ALIGN_CENTER, "NEW HIGHSCORE!");
	draw_text(SCREEN_WIDTH / 2, 150, ALIGN_CENTER, "ENTER YOUR NAME:");

	draw_text(SCREEN_WIDTH / 2, 250, ALIGN_CENTER, input_name);

	if (blink_cursor < FPS / 2)
	{
		SDL_Rect rect{ 0, 250, GLYPH_WIDTH, GLYPH_HEIGHT };
		rect.x = (SCREEN_WIDTH / 2) + (static_cast<int>(input_name.length()) * 
				 (GLYPH_WIDTH / 2)) + 5;
		SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(app.renderer, &rect);
	}

	draw_text(SCREEN_WIDTH / 2, 650, ALIGN_CENTER, "PRESS ENTER TO RETURN");
}

void draw_highscores()
{
	draw_text(SCREEN_WIDTH / 2, 75, ALIGN_CENTER, "HIGHSCORES");

	int y = 150;
	for (int i = 0; i < NUM_HIGHSCORES; ++i)
	{
		std::ostringstream oss;
		oss << (i + 1) << ". " << highscores[i].name << " . . . . . ";
		oss << highscores[i].score << '\n';
		if (highscores[i].recent)
		{
			draw_text(SCREEN_WIDTH / 2, y, ALIGN_CENTER, oss.str(), 
				255, 255, 0);
		}
		else
		{
			draw_text(SCREEN_WIDTH / 2, y, ALIGN_CENTER, oss.str());
		}
		y += 50;
	}

	draw_text(SCREEN_WIDTH / 2, 650, ALIGN_CENTER, "PRESS FIRE TO PLAY!");
}

bool operator<(const HighScore& a, const HighScore& b)
{
	return a.score < b.score;
}

int compare_highscores(const void* a, const void* b)
{
	// sort maximum first (high to low)
	if (*(HighScore*)a < *(HighScore*)b)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
