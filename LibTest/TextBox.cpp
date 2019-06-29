#include "TextBox.h"

#include <axeLib/util/Logger.h>

TextBox::TextBox(int x, int y, std::string font_path, int font_size, ALLEGRO_COLOR color) : 
	x(x), y(y), color(color)
{
	font = nullptr;
	font = al_load_font(font_path.c_str(), font_size, 0);

	if (!font)
	{
		axe::log(axe::LOGGER_ERROR, "Unable to load font: %s", font_path);
	}

	t.start();
}

TextBox::~TextBox()
{
	lines.clear();

	if (font) al_destroy_font(font);
}

void TextBox::insertString(std::string str)
{
	TextBoxLine line;
	line.str = str;
	line.alpha = 1.0f;
	line.timepoint = t.elapsed();

	lines.push_front(line);
}

void TextBox::update()
{
	if (lines.empty()) return;

	for (auto it = lines.rbegin(); it != lines.rend(); ++it)
	{
		if (t.elapsed() > (*it).timepoint + axe::m_secs(2000))
		{
			(*it).alpha -= 0.02f;
		}
	}

	if (!lines.empty() && lines.back().alpha <= 0.f)
	{
		lines.pop_back();
	}
}

void TextBox::draw()
{
	int line_height = al_get_font_line_height(font) + 2;

	int i = 0;

	float r, g, b;

	al_unmap_rgb_f(color, &r, &g, &b);

	for (auto &line : lines)
	{
		al_draw_text(font, al_map_rgba_f(r * line.alpha, g * line.alpha, b * line.alpha, line.alpha), x, y + i, 0, line.str.c_str());

		i += line_height;
	}
}