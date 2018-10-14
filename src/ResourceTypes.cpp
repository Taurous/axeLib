#include "axeLib\ResourceTypes.h"
#include <allegro5\allegro_primitives.h>

namespace axe
{

	// BITMAP FUNCTIONS

	Bitmap::~Bitmap()
	{
		destroy();
	}

	bool Bitmap::load()
	{
		if (!m_name.empty() && !m_path.empty())
		{
			std::string path = m_path + m_name;
			m_data = al_load_bitmap(path.c_str());

			if (m_data)
			{
				m_width = al_get_bitmap_width(m_data);
				m_height = al_get_bitmap_height(m_data);
				m_loaded = true;
			}
			else
			{
				axe::log(LOGGER_WARNING, "Cannot load resource %s%s\n", m_path.c_str(), m_name.c_str());
			}
		}
		else
		{
			axe::log(LOGGER_WARNING, "Cannot create resource with empty path!\n");
		}

		return m_loaded;
	}

	bool Bitmap::reload()
	{
		destroy();
		return load();
	}

	void Bitmap::destroy()
	{
		if (m_loaded && m_data)
		{
			al_destroy_bitmap(m_data);
			m_data = nullptr;
			m_loaded = false;
			m_width = 0;
			m_height = 0;
		}
	}

	void Bitmap::createDefault(void)
	{
		m_data = al_create_bitmap(32, 32);
		m_width = al_get_bitmap_width(m_data);
		m_height = al_get_bitmap_height(m_data);
		al_set_target_bitmap(m_data);
		al_draw_filled_triangle(1, 1, 32, 32, 1, 32, al_map_rgb(255, 0, 255));
		al_draw_filled_triangle(1, 1, 32, 1, 32, 32, al_map_rgb(0, 0, 0));
		al_draw_rectangle(1, 1, 32, 32, al_map_rgb(255, 0, 255), 1);
		al_set_target_backbuffer(al_get_current_display());
		m_loaded = true;
	}

	//SOUND FUNCTIONS -------------------------------------------- Needs work --------------------------------------------

	Sound::~Sound()
	{
		destroy();
	}

	bool Sound::load()
	{
		if (!m_name.empty() && !m_path.empty())
		{
			std::string path = m_path + m_name;
			m_data = al_load_sample(path.c_str());

			if (m_data)
			{
				axe::log(LOGGER_MESSAGE, "Loaded!\n");
				
				m_loaded = true;
			}
		}
		else
		{
			axe::log(LOGGER_WARNING, "Cannot create resource with empty path!\n");
		}

		return m_loaded;
	}

	bool Sound::reload()
	{
		destroy();
		return load();
	}

	void Sound::destroy()
	{
		if (m_loaded && m_data)
		{
			al_stop_sample(&m_sample_id);
			al_destroy_sample(m_data); // error
			m_data = nullptr;
			m_loaded = false;
		}
	}

	void Sound::createDefault(void)
	{
		m_loaded = false;
	}

	//FONT FUNCTIONS -------------------------------------------- Needs work --------------------------------------------

	Font::~Font()
	{
		destroy();
	}

	bool Font::load()
	{
		m_loaded = false;

		if (!m_name.empty() && !m_path.empty())
		{
			// ex name "18-arial.ttf"

			std::string fontSize = "";
			std::string temp_name = "";

			for (size_t i = 0; i < m_name.size(); ++i)
			{
				if (m_name[i] == '-')
				{
					i += 1;
					temp_name = m_name.substr(i, m_name.size() - i);
					break;
				}
				if (m_name[i] >= '0' && m_name[i] <= '9')
				{
					fontSize += m_name[i];
				}
			}

			int size = 18;

			try
			{
				size = std::stoi(fontSize);
			}
			catch (const std::invalid_argument& ia)
			{
				axe::log(axe::LOGGER_ERROR, "Font Loader - EXCEPTION: Invalid Argument: %s\n", ia.what());
			}
			catch (const std::out_of_range& oor)
			{
				axe::log(axe::LOGGER_ERROR, "Font Loader - EXCEPTION: Out of Range: %s\n", oor.what());
			}

			std::string path = m_path + temp_name;
			m_data = al_load_font(path.c_str(), size, 0);

			if (m_data)
			{
				m_font_height = al_get_font_line_height(m_data);
				m_loaded = true;
			}
		}
		else
		{
			axe::log(LOGGER_WARNING, "Cannot create resource with empty path!\n");
		}

		return m_loaded;
	}

	bool Font::reload()
	{
		destroy();
		return load();
	}

	void Font::destroy()
	{
		if (m_loaded && m_data)
		{
			al_destroy_font(m_data);
			m_data = nullptr;
			m_loaded = false;
		}
	}

	void Font::createDefault(void)
	{
		m_name = "ariblk.ttf";
		m_path = "C:\\Windows\\Fonts\\Arial\\";
		m_loaded = false;
	}

	unsigned int Font::getStringWidth(std::string str)
	{
		if (!m_loaded) return 0;
		// Calculate width of entire string (done in parts as al_get_text_width() cannot handle large strings)
		unsigned int substring_size = 50;
		unsigned int string_length = 0;
		for (unsigned int i = 0; i < str.size(); i += substring_size)
		{
			if (i + substring_size > str.size())
			{
				string_length += al_get_text_width(m_data, str.substr(i, str.size() - i).c_str());
				break;
			}
			else
			{
				string_length += al_get_text_width(m_data, str.substr(i, substring_size).c_str());
			}
		}

		return string_length;
	}
}