#include "axeLib/Window.h"

namespace axe
{
	Window::Window()
		: m_width(0)
		, m_height(0)
		, m_title("")
		, m_icon_path("")
		, m_flags(0)
		, m_display(nullptr)
		, m_event_queue(nullptr)
		, m_icon(nullptr)
		, m_aspect_ratio(1)
	{
		
	}

	Window::~Window()
	{
		if (m_icon) al_destroy_bitmap(m_icon);
		if (m_display) al_destroy_display(m_display);

		m_icon = nullptr;
		m_display = nullptr;
	}

	Window &Window::create(int width, int height, ALLEGRO_EVENT_QUEUE *eq, int flags)
	{
		m_width = width;
		m_default_width = width;
		m_height = height;
		m_default_height = height;
		m_flags = flags;
		m_event_queue = eq;

		m_aspect_ratio = double(m_width) / double(m_height);

		//Removing ability to detect multiple displays for now.

		al_get_monitor_info(0, &m_monitor_info); // Get Monitor dimensions

		int num_display_modes = al_get_num_display_modes(); // Get number of different display resolutions and bitrates
		for (int i = 0; i < num_display_modes; ++i)
		{
			ALLEGRO_DISPLAY_MODE disp_mode;
			al_get_display_mode(i, &disp_mode);
			m_display_modes.push_back(disp_mode);
		}

		createWindow();

		return *this;
	}

	void Window::destroy()
	{
		al_unregister_event_source(m_event_queue, al_get_display_event_source(m_display));
		al_destroy_display(m_display);
	}


	void Window::printDisplayModes()
	{
		int i = 0;
		for (const auto &mode : m_display_modes)
		{
			printf("Display Mode %i\n", i);
			printf("Resolution: %ix%i\n", mode.width, mode.height);
			printf("Aspect Ratio: %.3f\n", float(mode.width) / float(mode.height));
			printf("Format: %i\n", mode.format);
			printf("Refresh Rate: %i\n\n", mode.refresh_rate);
			++i;
		}
	}

	void Window::createWindow()
	{
		al_set_new_display_flags(m_flags);

		m_display = al_create_display(m_width, m_height);

		if (!m_display)
		{
			axe::crash("Unable to create display at resolution %ix%i", m_width, m_height);
		}

		if (m_flags & ALLEGRO_MAXIMIZED)
		{
			resized();
			printf("createWindow() -> resized()\n");
		}

		al_register_event_source(m_event_queue, al_get_display_event_source(m_display));

		if (!getFullscreen()) m_width = al_get_display_width(m_display); // Get actual display resoltuion (if created with ALLEGRO_FULLSCREEN_WINDOW, passed in resolution is ignored and window is set to size of desktop)
		if (!getFullscreen()) m_height = al_get_display_height(m_display);
	}

	Window &Window::setWindowIcon(std::string path)
	{
		if (m_flags & ALLEGRO_WINDOWED && !path.empty())
		{
			if (m_icon) al_destroy_bitmap(m_icon);

			m_icon = al_load_bitmap(path.c_str());

			if (m_icon) al_set_display_icon(m_display, m_icon);
			else
			{
				axe::log(LOGGER_WARNING, "Unable to load icon: %s\n", path.c_str());
				m_icon = nullptr;
			}
		}

		return *this;
	}

	Window &Window::setWindowTitle(std::string title)
	{
		if (m_flags & ALLEGRO_WINDOWED)
		{
			al_set_window_title(m_display, title.c_str());
		}

		return *this;
	}

	void Window::setFullscreen(bool f)
	{
		int mask = ~(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_WINDOWED); // 0b0111111110
		m_flags = m_flags & mask; // Both fullscreen and windowed flags have been cleared

		if (f)  m_flags = m_flags | ALLEGRO_FULLSCREEN_WINDOW; // Set fullscreen flag
		else m_flags = m_flags | ALLEGRO_WINDOWED;

		if (m_flags & ALLEGRO_MAXIMIZED)
		{
			m_width = m_default_width;
			m_height = m_default_height;
		}

		destroy();
		createWindow();
	}

	void Window::resized()
	{
		al_acknowledge_resize(m_display);
		m_width = al_get_display_width(m_display);
		m_height = al_get_display_height(m_display);

		al_convert_memory_bitmaps();
	}

	void Window::centerWindow(void)
	{
		al_set_window_position(m_display, getScreenWidth() / 2 - m_width / 2, getScreenHeight() / 2 - m_height / 2);
	}
}