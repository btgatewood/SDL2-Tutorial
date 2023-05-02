#include "main.h"

SDL_Texture* font_texture;

void init_font()
{
	font_texture = load_texture("data/font.png");
}

void draw_text(int x, int y, const std::string& str, int r, int g, int b)
{
	SDL_SetTextureColorMod(font_texture, r, g, b);  // defaults to 255, 255, 255

	SDL_Rect glyph_rect{ 0, 0, GLYPH_WIDTH, GLYPH_HEIGHT };
	for (const char& ch : str)
	{
		int c = static_cast<int>(ch);

		if (c >= ' ' && c <= 'Z')
		{
			glyph_rect.x = (c - ' ') * GLYPH_WIDTH;
			draw_rect(font_texture, glyph_rect, x, y);
			x += GLYPH_WIDTH;
		}
	}
}
