#include "main.h"

SDL_Texture* font_texture;

void init_font()
{
	font_texture = load_texture("data/font.png");
}

// NOTE: text drawing does not support lowercase letters
void draw_text(int x, int y, int align, const std::string& str, 
			   int r, int g, int b)
{
	SDL_SetTextureColorMod(font_texture, r, g, b);  // defaults to 255, 255, 255

	SDL_Rect glyph_rect{ 0, 0, GLYPH_WIDTH, GLYPH_HEIGHT };

	switch (align)
	{
	case ALIGN_RIGHT:  // TODO: test right alignment
		x -= static_cast<int>(str.size() * GLYPH_WIDTH);
		break;
	case ALIGN_CENTER:
		x -= static_cast<int>(str.size() * GLYPH_WIDTH) / 2;
		break;
	}

	for (const char& ch : str)
	{
		// int c = static_cast<int>(ch);
		if (ch >= ' ' && ch <= 'Z')
		{
			glyph_rect.x = (ch - ' ') * GLYPH_WIDTH;
			draw_rect(font_texture, glyph_rect, x, y);
			x += GLYPH_WIDTH;
		}
	}
}
