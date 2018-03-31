#pragma once

#include "axeEngine\GUI\GUIObject.h"
#include "axeEngine\InputHandler.h"
#include "axeEngine\EventHandler.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

namespace axe
{
	class Frame : public GUIObject
	{
	protected:
		bool m_locked;
		int m_diff_x;
		int m_diff_y;

		ALLEGRO_COLOR m_background;
		ALLEGRO_COLOR m_border;

	public:
		Frame(DrawEngine *draw, GUIObject *parent, int x, int y, int width, int height, int anchor = ANCHOR_TOPLEFT);
		virtual ~Frame() { }

		virtual bool handleEvents(const InputHandler &input, const EventHandler &events);
		virtual void draw();

		virtual void setBackgroundColor(const ALLEGRO_COLOR &cl) { m_background = cl; }
		virtual void setBorderColor(const ALLEGRO_COLOR &cl) { m_border = cl; }

		void setLocked(bool flag) { m_locked = flag; }
	};
}