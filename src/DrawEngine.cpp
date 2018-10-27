#include "axeLib/DrawEngine.h"
#include "axeLib/util/Util.h"

using namespace axe;

DrawEngine::DrawEngine()
{
	axe::allegro_init();

	al_init_image_addon();
	al_init_primitives_addon();

	log(LOGGER_MESSAGE, "DrawEngine Initialized\n");
}

DrawEngine::~DrawEngine()
{

}

void axe::DrawEngine::handleEvents(const ALLEGRO_EVENT & ev)
{
	if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
	{
		m_window.resized();
	}
}

Window &DrawEngine::createWindow(int width, int height, std::string title, std::string icon_path, int flags)
{
	return m_window.create(width, height, title, icon_path, flags);
}

/*

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

*/