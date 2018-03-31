#include "axeEngine\GUI\Button.h"

using namespace axe;

Button::Button(DrawEngine *draw, GUIObject *parent, int x, int y, int width, int height, const std::string & label, int anchor)
	: GUIInteractable(draw, parent, x, y, width, height, anchor),
	m_label(label)
{
	frame = new Frame(draw, this, 0, 0, width, height);
}

Button::~Button()
{

}

void Button::setFont(const std::string &path)
{
	if (font)
	{
		m_draw->fonts[font]->setName(path);
		m_draw->fonts[font]->reload();
	}
	else
	{
		font = m_draw->fonts.getResource(path);
	}

	if (!font) axe::log(axe::LOGGER_WARNING, "GUI Object id %i unable to load font %s\n", m_id, path.c_str());
}

void Button::handleEvents(const InputHandler &input, EventHandler &events)
{
	if (keybind)
	{
		if (input.isKeyPressed(keybind) && state != DISABLED)
		{
			state = PRESSED;
			frame->setBackgroundColor(m_background_pressed);
			frame->setBorderColor(m_border_pressed);
			events.emitEvent(GUI_EVENT_BUTTON_PRESSED, m_id);
		}
		else if (input.isKeyReleased(keybind) && state != DISABLED)
		{
			state = NORMAL;
			frame->setBackgroundColor(m_background_normal);
			frame->setBorderColor(m_border_normal);
			events.emitEvent(GUI_EVENT_BUTTON_RELEASED, m_id);
		}

		return; // keybind completely overrides mouse with this line
	}

	if (state != DISABLED)
	{
		if (input.isMousePressed(axe::MOUSE_LEFT) && m_hovered)
		{
			state = PRESSED;
			frame->setBackgroundColor(m_background_pressed);
			frame->setBorderColor(m_border_pressed);
			events.emitEvent(GUI_EVENT_BUTTON_PRESSED, m_id);
		}
		else if (input.isMouseReleased(axe::MOUSE_LEFT))
		{
			state = NORMAL;

			if (m_hovered)
			{
				frame->setBackgroundColor(m_background_hover);
				frame->setBorderColor(m_border_hover);
				events.emitEvent(GUI_EVENT_BUTTON_RELEASED, m_id);
			}
			else
			{
				// Fire released, but not pressed event?
				frame->setBackgroundColor(m_background_normal);
				frame->setBorderColor(m_border_normal);
			}
		}
		else if (events.eventIs(ALLEGRO_EVENT_MOUSE_AXES))
		{
			int mouse_x = input.getMouseX();
			int mouse_y = input.getMouseY();

			if (mouse_x >= m_screen_x && mouse_x <= m_screen_x + m_width && mouse_y >= m_screen_y && mouse_y <= m_screen_y + m_height)
			{
				if (!m_hovered && state == NORMAL)
				{
					m_hovered = true;
					frame->setBackgroundColor(m_background_hover);
					frame->setBorderColor(m_border_hover);
				}
				else if (!m_hovered && state == PRESSED)
				{
					m_hovered = true;
					frame->setBackgroundColor(m_background_pressed);
					frame->setBorderColor(m_border_pressed);
				}
			}
			else
			{
				if (m_hovered)
				{
					m_hovered = false;
					/*if (state == PRESSED)
					{
						frame->setBackgroundColor(m_background_pressed);
						frame->setBorderColor(m_border_pressed);
					}
					else
					{*/
						frame->setBackgroundColor(m_background_normal);
						frame->setBorderColor(m_border_normal);
					//
				}
			}
		}
	}
}

void Button::draw(void)
{
	if (m_visible)
	{
		int mid_x = m_screen_x + (m_width / 2);
		int mid_y = m_screen_y + (m_height / 2);

		for (GUIObject *it : m_children)
		{
			it->draw();
		}

		//al_draw_text(font, al_map_rgb(255, 255, 0), mid_x, mid_y - line_height + m_pressed, ALLEGRO_ALIGN_CENTER, m_label.c_str());
		m_draw->drawText(font, al_map_rgb(255, 255, 0), mid_x, mid_y - (m_draw->fonts[font]->getFontHeight()  / 2) + (state==PRESSED), ALLEGRO_ALIGN_CENTER, m_label.c_str());
	}
}

bool axe::Button::isPressed(void)
{
	return (state == PRESSED && (m_hovered || keybind));
}

void axe::Button::setEnabled(bool enable)
{
	if (!enable)
	{
		state = DISABLED;
		frame->setBackgroundColor(m_background_disabled);
		frame->setBorderColor(m_border_disabled);
	}
	else
	{
		state = NORMAL;
		frame->setBackgroundColor(m_background_normal);
		frame->setBorderColor(m_border_normal);
	}
}

void axe::Button::setBackgroundColors(const ALLEGRO_COLOR & normal, const ALLEGRO_COLOR & pressed, const ALLEGRO_COLOR & disabled, const ALLEGRO_COLOR & hover)
{
	frame->setBackgroundColor(normal);
	m_background_normal = normal;
	m_background_pressed = pressed;
	m_background_hover = hover;
	m_background_disabled = disabled;
}

void axe::Button::setBackgroundColors(const ALLEGRO_COLOR & cl)
{
	frame->setBackgroundColor(cl);
	m_background_normal = cl;
	m_background_pressed = cl;
	m_background_hover = cl;
	m_background_disabled = cl;
}

void axe::Button::setBorderColors(const ALLEGRO_COLOR & normal, const ALLEGRO_COLOR & pressed, const ALLEGRO_COLOR & disabled, const ALLEGRO_COLOR & hover)
{
	frame->setBorderColor(normal);
	m_border_normal = normal;
	m_border_pressed = pressed;
	m_border_hover = hover;
	m_border_disabled = disabled;
}