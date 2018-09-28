#pragma once

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>

#include <vector>
#include <string>

#include "Window.h"

#include "util\Logger.h"

#include "DrawObjectRef.h"

namespace axe
{
	class DrawEngine
	{
	public:
		DrawEngine();

		DrawEngine(const DrawEngine &other) = delete;
		DrawEngine& operator=(const DrawEngine &other) = delete;

		DrawEngine(DrawEngine &&other) = delete;
		DrawEngine& operator=(DrawEngine &&other) = delete;

		~DrawEngine();

		Window &createWindow(int width, int height, std::string title, std::string icon_path = "", int flags = ALLEGRO_WINDOWED);
		Window &getWindow() { return m_window; }

		void flipAndClear(ALLEGRO_COLOR cl);

	private:
		Window m_window;
	};
};