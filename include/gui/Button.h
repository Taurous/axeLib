#pragma once

#include "GUIInteractable.h"
#include "frame.h"

namespace axe
{
	class Button : public GUIInteractable
	{
	public:
		Button(DrawEngine *draw, GUIObject *parent, int x, int y, int width, int height, const std::string &label, int anchor = ANCHOR_TOPLEFT);
		virtual ~Button();

		virtual void handleEvents(const InputHandler &input, EventHandler &events);
		virtual void draw();

		void setLabel(const std::string &label) { m_label = label; }
		bool isPressed(void);
		virtual void setEnabled(bool enable);

		void setBackgroundColors(const ALLEGRO_COLOR &normal, const ALLEGRO_COLOR &pressed, const ALLEGRO_COLOR &disabled, const ALLEGRO_COLOR &hover);
		void setBackgroundColors(const ALLEGRO_COLOR &cl);
		void setBorderColors(const ALLEGRO_COLOR &normal, const ALLEGRO_COLOR &pressed, const ALLEGRO_COLOR &disabled, const ALLEGRO_COLOR &hover);
		void setFont(const std::string &path);

	private:
		Frame *frame;

		ALLEGRO_COLOR m_background_disabled,	m_border_disabled;
		ALLEGRO_COLOR m_background_hover,		m_border_hover;
		ALLEGRO_COLOR m_background_pressed,		m_border_pressed;
		ALLEGRO_COLOR m_background_normal,		m_border_normal;

		std::string m_label;

		ResourceHandle<Font> font;
	};
}