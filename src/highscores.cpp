#include "main.h"

extern App app;

std::vector<HighScore> highscores;

void init_highscore_table()
{
	for (int i = 0; i < NUM_HIGHSCORES; ++i)
	{
		highscores.push_back(HighScore{ NUM_HIGHSCORES - i, 0 });  // desc order
	}
}

static void update()
{
	update_background();

	if (app.keyboard[SDL_SCANCODE_LCTRL] || app.keyboard[SDL_SCANCODE_SPACE] ||
		app.keyboard[SDL_SCANCODE_RCTRL])
	{
		init_scene();
	}
}

void draw_highscores()
{
	draw_text(425, 70, "HIGHSCORES");

	int y = 150;
	for (int i = 0; i < NUM_HIGHSCORES; ++i)
	{
		std::ostringstream oss;
		oss << (i + 1) << " . . . . . . . . . . . . . . . . ";
		oss << highscores[i].score << '\n';
		if (highscores[i].recent)
		{
			draw_text(425, y, oss.str(), 255, 255, 0);
		}
		else
		{
			draw_text(425, y, oss.str());
		}
		y += 50;
	}

	draw_text(425, 600, "PRESS FIRE TO PLAY!");
}

static void render()
{
	draw_background();
	draw_highscores();
}

void init_highscores()
{
	app.delegate.update = update;
	app.delegate.render = render;

	std::fill_n(app.keyboard, NUM_KEYS, 0);  // clear key states
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

void add_highscore(int score)
{
	for (HighScore& highscore : highscores)
	{
		highscore.recent = false;
	}

	highscores.push_back(HighScore{ score, true });  // add recent score

	// sort the recent score into the highscore list
	std::qsort(highscores.data(), highscores.size(), sizeof(HighScore),
		compare_highscores);

	highscores.resize(NUM_HIGHSCORES);
}