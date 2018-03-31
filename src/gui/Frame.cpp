#include "axeEngine\GUI\Frame.h"

namespace axe
{
	Frame::Frame(DrawEngine *draw, GUIObject * parent, int x, int y, int width, int height, int anchor)
		: GUIObject(draw, parent, x, y, width, height, anchor), m_locked(false), m_diff_x(-1), m_diff_y(-1)
	{
		m_background = al_map_rgba(0, 0, 0, 200);
		m_border = al_map_rgb(0, 0, 0);
	}

	bool Frame::handleEvents(const InputHandler &input, const EventHandler &events)
	{
		if (!m_locked)
		{
			int mouse_x = input.getMouseX();
			int mouse_y = input.getMouseY();

			if (mouse_x > m_screen_x && mouse_x < m_screen_x + m_width &&
				mouse_y > m_screen_y && mouse_y < m_screen_y + m_height)
			{
				if (input.isMousePressed(MOUSE_RIGHT))
				{
					m_diff_x = mouse_x - m_screen_x;
					m_diff_y = mouse_y - m_screen_y;
				}

				if (input.isMouseDown(MOUSE_RIGHT) && events.eventIs(ALLEGRO_EVENT_MOUSE_AXES) && m_diff_x >= 0 && m_diff_y >= 0)
				{
					int xx = mouse_x - m_diff_x;
					int yy = mouse_y - m_diff_y;

					if (xx < 0) xx = 1;
					if (yy < 0) yy = 1;
					if (xx > m_draw->getWindowWidth() - m_width) xx = m_draw->getWindowWidth() - m_width;
					if (yy > m_draw->getWindowHeight() - m_height) yy = m_draw->getWindowHeight() - m_height;

					moveTo(xx, yy);
				}
			}

			if (input.isMouseReleased(MOUSE_RIGHT))
			{
				m_diff_x = -1;
				m_diff_y = -1;
			}
		}

		return false;
	}

	void Frame::draw(void)
	{
		if (m_visible)
		{
			al_draw_filled_rectangle(m_screen_x, m_screen_y, m_screen_x + m_width, m_screen_y + m_height, m_background);
			al_draw_rectangle(m_screen_x, m_screen_y, m_screen_x + m_width, m_screen_y + m_height, m_border, 1);

			for (GUIObject *child : m_children)
			{
				child->draw();
			}
		}
	}
}