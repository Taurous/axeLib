#pragma once

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>

#include <vector>
#include <string>

#include "axeLib\Window.h"

#include "axeLib/util/Logger.h"

namespace axe
{
	void flipAndClear(ALLEGRO_COLOR cl);

	class DrawEngine
	{
	public:
		DrawEngine();

		DrawEngine(const DrawEngine &other) = delete;
		DrawEngine& operator=(const DrawEngine &other) = delete;

		DrawEngine(DrawEngine &&other) = delete;
		DrawEngine& operator=(DrawEngine &&other) = delete;

		~DrawEngine();

		void handleEvents(const ALLEGRO_EVENT &ev);

		Window &createWindow(int width, int height, std::string title, std::string icon_path = "", int flags = ALLEGRO_WINDOWED);
		Window &getWindow() { return m_window; }

	private:
		Window m_window;
	};
};