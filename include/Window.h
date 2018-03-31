#pragma once

#include "Util\Logger.h"
#include <allegro5\allegro.h>
#include <string>
#include <vector>

namespace axe
{
	class Window
	{
	public:
		Window();

		Window(const Window &other) = delete;
		Window &operator=(const Window &other) = delete;

		Window(Window &&other) = delete;
		Window &operator=(Window &&other) = delete;

		~Window();

		void create(int width, int height, std::string title, std::string icon_path = "", int flags = ALLEGRO_WINDOWED);

		void setWindowIcon(std::string path);
		void setWindowTitle(std::string title);

		bool getFullscreen() const { return (m_flags & ALLEGRO_FULLSCREEN || m_flags & ALLEGRO_FULLSCREEN_WINDOW); }

		int getScreenWidth() const { return m_monitor_info.x2 - m_monitor_info.x1; } // Returns size of monitor
		int getScreenHeight() const { return m_monitor_info.y2 - m_monitor_info.y1; } // Returns size of monitor

		int getWindowWidth() const { return m_width; } // Returns size of window, not monitor
		int getWindowHeight() const { return m_height; } // Returns size of window, not monitor

		void registerForEvents(ALLEGRO_EVENT_QUEUE *eq);
		ALLEGRO_DISPLAY *getAllegroDisplay() { return m_display; }

		void centerWindow(); // Untested

		void printDisplayModes();

	private:
		void createWindow();
		
		ALLEGRO_MONITOR_INFO m_monitor_info;
		std::vector<ALLEGRO_DISPLAY_MODE> m_display_modes;

		ALLEGRO_DISPLAY *m_display;
		ALLEGRO_BITMAP *m_icon;
		ALLEGRO_EVENT_QUEUE *m_event_queue;

		std::string m_title;
		std::string m_icon_path;

		int m_flags;
		int m_width;
		int m_height;

		int m_current_display_mode;
	};
}