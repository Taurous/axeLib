#include "axeEngine\GUI\ScrollingTextFrame.h"

/* Implemented scrolling/scroll bar. Needs to be cleaned up and accessor function created */

const int TEXT_TOP = 0;

namespace axe
{
	ScrollingTextFrame::ScrollingTextFrame(DrawEngine *draw, GUIObject * parent, int x, int y, int width, int height, int anchor) : Frame(draw, parent, x, y, width, height, anchor),
		scroll_speed(0), scroll_bar_height(0), scroll_bar_width(8), text_height(0), position(0),
		scroll_bar_position(0), scroll_bar_padding(4), text_padding(2)
	{
		timer.start();
	}

	ScrollingTextFrame::~ScrollingTextFrame()
	{
		strings.clear();
	}

	void ScrollingTextFrame::update(const InputHandler &input)
	{
		if (text_height < m_height) // if text height is smaller than the frame, no need to worry about scrolling
		{
			position = TEXT_TOP;
			return;
		}

		if (input.isKeyDown(ALLEGRO_KEY_UP) && timer.elapsed() >= m_secs(5))
		{
			if (position - scroll_speed > TEXT_TOP)
			{
				position -= scroll_speed;
			}
			else
			{
				position = TEXT_TOP;
			}

			calculateScrollBarPosition();

			timer.start();
		}
		else if (input.isKeyDown(ALLEGRO_KEY_DOWN) && timer.elapsed() >= m_secs(5))
		{
			if (position + scroll_speed < getTextBottom())
			{
				position += scroll_speed;
			}
			else
			{
				position = getTextBottom();
			}

			calculateScrollBarPosition();

			timer.start();
		}
	}

	bool ScrollingTextFrame::handleEvents(const InputHandler &input, const EventHandler &events)
	{
		bool key_pressed = false;

		if (input.isKeyPressed(ALLEGRO_KEY_RIGHT, MOD_NONE))
		{
			position += m_draw->fonts[font]->getFontHeight() * 8; // get rid of magic number
			if (position > text_height - m_height) position = getTextBottom();
			key_pressed = true;
		}
		else if (input.isKeyPressed(ALLEGRO_KEY_RIGHT, MOD_SHIFT))
		{
			position = getTextBottom();
			key_pressed = true;
		}
		else if (input.isKeyPressed(ALLEGRO_KEY_LEFT, MOD_NONE))
		{
			position -= m_draw->fonts[font]->getFontHeight() * 8; // get rid of magic number
			if (position < TEXT_TOP) position = TEXT_TOP;
			key_pressed = true;
		}
		else if (input.isKeyPressed(ALLEGRO_KEY_LEFT, MOD_SHIFT))
		{
			position = TEXT_TOP;
			key_pressed = true;
		}

		if (key_pressed) calculateScrollBarPosition();

		return key_pressed;
	}

	void ScrollingTextFrame::draw()
	{
		if (m_visible)
		{
			// Draw Frame
			al_draw_filled_rectangle(m_screen_x, m_screen_y, m_screen_x + m_width, m_screen_y + m_height, m_background);
			al_draw_rectangle(m_screen_x, m_screen_y, m_screen_x + m_width, m_screen_y + m_height, m_border, 1);

			// Create Clipping Rect
			int cx, cy, cw, ch;
			int nx, ny, nw, nh;

			nx = m_screen_x + text_padding;
			ny = m_screen_y + text_padding;

			nw = m_width - scroll_bar_padding - scroll_bar_width - (2 * text_padding);
			nh = m_height - (2 * text_padding);

			al_get_clipping_rectangle(&cx, &cy, &cw, &ch);
			al_set_clipping_rectangle(nx, ny, nw, nh);

			// Draw Strings
			for (auto it = strings.begin(); it != strings.end(); ++it)
			{
				int y = m_screen_y - position + text_padding + (*it).position;
				m_draw->drawTextNewLine(font, (*it).cl, m_screen_x + text_padding, y, 0, (*it).flags, (*it).string);
			}

			al_set_clipping_rectangle(cx, cy, cw, ch);

			// Draw Scroll Bar
			if (text_height > m_height) al_draw_rectangle(m_screen_x + m_width - scroll_bar_padding - scroll_bar_width, scroll_bar_position,
				m_screen_x + m_width - scroll_bar_padding, scroll_bar_position + scroll_bar_height, m_border, 1);

			// Draw Children
			for (GUIObject *child : m_children)
			{
				child->draw();
			}
		}
	}

	void ScrollingTextFrame::insertString(ALLEGRO_COLOR cl, int flags, std::string str)
	{
		TextObj obj;
		obj.cl = cl;
		obj.flags = flags;
		obj.rows = m_draw->formatStringByWidth(font, m_width - (2 * text_padding) - scroll_bar_width - scroll_bar_padding, str);

		// Add string to list of strings to draw
		obj.string = str;

		if (strings.empty())
		{
			obj.position = TEXT_TOP;
		}
		else
		{
			int h = m_draw->fonts[font]->getFontHeight() * strings.back().rows + (m_draw->fonts[font]->getFontHeight() / 2);
			obj.position = strings.back().position + h;
		}

		if (text_height < obj.position + (obj.rows * m_draw->fonts[font]->getFontHeight())) text_height = obj.position + (obj.rows * m_draw->fonts[font]->getFontHeight());

		position = getTextBottom();

		calculateScrollBarPosition();

		strings.push_back(obj);
	}

	void ScrollingTextFrame::setFont(std::string file)
	{
		if (font)
		{
			m_draw->fonts[font]->setName(file);
			m_draw->fonts[font]->reload();
		}
		else
		{
			font = m_draw->fonts.getResource(file);
		}

		if (!font) axe::log(axe::LOGGER_WARNING, "GUI Object id %i unable to load font %s\n", m_id, file.c_str());
		else
		{
			calculateScrollBarPosition();
			scroll_speed = m_draw->fonts[font]->getFontHeight() / 2;
		}
	}

	int ScrollingTextFrame::getTextBottom(void)
	{
		return text_height - m_height + (2 * text_padding);
	}

	void ScrollingTextFrame::calculateScrollBarPosition()
	{
		float percent = float(position) / float(getTextBottom()); // percentage of the screen height the bar should be at
		scroll_bar_height = m_height * (float(m_height) / float(text_height)) - (2 * scroll_bar_padding);
		scroll_bar_position = m_screen_y + scroll_bar_padding + (percent * float((m_screen_y + m_height - scroll_bar_padding - scroll_bar_height) - (m_screen_y + scroll_bar_padding))); // calculate actual on screen position
	}
}