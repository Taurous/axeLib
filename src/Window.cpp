#include "axeLib/Window.h"

namespace axe
{
	Window::Window()
		: m_width(0), m_default_width(0)
		, m_height(0), m_default_height(0)
		, m_title("")
		, m_icon_path("")
		, m_flags(0)
		, m_fullscreen(false)
		, m_display(nullptr)
		, m_event_queue(nullptr)
		, m_icon(nullptr)
	{
		
	}

	Window::~Window()
	{
		destroy();
	}

	Window &Window::create(int width, int height, ALLEGRO_EVENT_QUEUE *eq, bool fullscreen, int flags)
	{
		if (m_display != nullptr) return *this;

		m_width = width;
		m_default_width = width;
		m_height = height;
		m_default_height = height;
		m_flags = flags;
		m_event_queue = eq;
		m_fullscreen = fullscreen;

		al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE | m_flags);

		al_get_monitor_info(0, &m_monitor_info); // Get Monitor dimensions

		// Unneeded if not using actual fullscreen.
		int num_display_modes = al_get_num_display_modes(); // Get number of different display resolutions and bitrates
		for (int i = 0; i < num_display_modes; ++i)
		{
			ALLEGRO_DISPLAY_MODE disp_mode;
			al_get_display_mode(i, &disp_mode);
			m_display_modes.push_back(disp_mode);
		}

		m_display = al_create_display(m_width, m_height);

		if (!m_display)
		{
			axe::crash("Unable to create display at resolution %ix%i", m_width, m_height);
		}

		al_register_event_source(m_event_queue, al_get_display_event_source(m_display));

		//resized(); // Need to resize after creating window in case dimensions are changed by Allegro (allegro bug)

		if (!m_fullscreen)
		{
			centerWindow();
		}
		else
		{
			setFullscreen(fullscreen);
		}

		return *this;
	}

	void Window::destroy()
	{
		if (m_display)
		{
			al_unregister_event_source(m_event_queue, al_get_display_event_source(m_display));
			al_destroy_display(m_display);
		}

		if (m_icon) al_destroy_bitmap(m_icon);

		m_icon = nullptr;
		m_display = nullptr;

		//Leave m_title and m_icon_path intact in case we are destroying the display only to recreate
	}


	void Window::printDisplayModes()
	{
		int i = 0;
		for (const auto &mode : m_display_modes)
		{
			printf("Display Mode %i\n", i);
			printf("\tResolution: %ix%i\n", mode.width, mode.height);
			printf("\tAspect Ratio: %.3f\n", float(mode.width) / float(mode.height));
			printf("\tFormat: %i\n", mode.format);
			printf("\tRefresh Rate: %i\n\n", mode.refresh_rate);
			++i;
		}
	}

	Window &Window::setWindowIcon(std::string path)
	{
		if (path.empty()) return *this;

		m_icon_path = path;

		if (m_icon) al_destroy_bitmap(m_icon);

		m_icon = al_load_bitmap(path.c_str());

		if (m_icon) al_set_display_icon(m_display, m_icon);
		else
		{
			axe::log(LOGGER_WARNING, "Unable to load icon: %s\n", path.c_str());
			m_icon = nullptr;
		}

		return *this;
	}

	Window &Window::setWindowTitle(std::string title)
	{
		if (title.empty()) return *this;

		m_title = title;

		al_set_window_title(m_display, title.c_str());

		return *this;
	}

	void Window::setFullscreen(bool f)
	{
		al_toggle_display_flag(m_display, ALLEGRO_FULLSCREEN_WINDOW, f);
		m_fullscreen = f;
		resized();
	}

	void Window::resized()
	{
		printf("Resizing from %ix%i...\n", m_width, m_height);
		al_acknowledge_resize(m_display);
		m_width = al_get_display_width(m_display);
		m_height = al_get_display_height(m_display);
		printf("Resized to %ix%i\n", m_width, m_height);
	}

	void Window::centerWindow(void)
	{
		int x_pos = getScreenWidth() / 2;
		x_pos -= getWindowWidth() / 2;
		x_pos = x_pos > 0 ? x_pos : 0;

		int y_pos = getScreenHeight() / 2;
		y_pos -= getWindowHeight() / 2;
		y_pos = y_pos > 0 ? y_pos : 0;

		al_set_window_position(m_display, x_pos, y_pos);
	}
}