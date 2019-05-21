#pragma once

#include "axeLib\Window.h"

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

		Window &createWindow(int width, int height, ALLEGRO_EVENT_QUEUE *eq, bool fullscreen, int flags = 0);
		Window &getWindow() { return m_window; }

	private:
		Window m_window;
	};
};