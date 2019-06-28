#pragma once

#include <string>
#include <deque>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include <axeLib/util/Timer.h>

struct TextBoxLine
{
	std::string str;
	axe::m_secs timepoint;
	float alpha;
};

class TextBox
{
public:
	TextBox(int x, int y, std::string font_path, int font_size, ALLEGRO_COLOR text_color);
	~TextBox();

	void insertString(std::string str);

	void draw();

	void update();

private:
	ALLEGRO_FONT *font;
	ALLEGRO_COLOR color;

	axe::Timer t;

	int x, y;
	std::deque<TextBoxLine> lines;
};