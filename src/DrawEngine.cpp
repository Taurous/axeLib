#include "DrawEngine.h"

using namespace axe;

DrawEngine::DrawEngine()
{
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	bitmaps.setPathToResources("resources/textures/");	// Default Path
	fonts.setPathToResources("resources/fonts/");		// Default Path

	t_cursorFlash.start();

	printf("DrawEngine Initialized\n");
}

DrawEngine::~DrawEngine()
{
	
}

void DrawEngine::createWindow(int width, int height, std::string title, std::string icon_path, int flags)
{
	m_window.create(width, height, title, icon_path, flags);
}
void axe::DrawEngine::handleEvents(const ALLEGRO_EVENT &ev)
{

}

int DrawEngine::formatStringByWidth(const ResourceHandle<Font> &font, int max_string_width, std::string &str)
{
	int rows = 1;
	int string_length = fonts[font]->getStringWidth(str);

	// If String is too big to fit in one row, insert newlines to break it up
	if (string_length > max_string_width)
	{
		string_length = 0;
		int last_space = -1;
		int word_length = 0;
		int last_length = 0;

		for (unsigned int i = 0; i < str.size(); ++i)
		{
			if (str[i] == ' ')
			{
				word_length = fonts[font]->getStringWidth(str.substr(last_space + 1, i - last_space)); // Calculate width of last word

				if (string_length + word_length - last_length > max_string_width)
				{
					str[last_space] = '\n'; // convert previous space to newline
					last_length = string_length;
					string_length += word_length;
					rows++;
				}
				else
				{
					string_length += word_length;
				}

				last_space = i;
			}
		}
	}

	return rows;
}