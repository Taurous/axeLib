#pragma once

#include <vector>

#include "axeEngine\InputHandler.h"
#include "axeEngine\ResourceManager.h"

#include "button.h"
#include "frame.h"

namespace axe
{
	class ScrollingTextFrame : public Frame
	{
	public:
		ScrollingTextFrame(DrawEngine *draw, GUIObject *parent, int x, int y, int width, int height, int anchor = ANCHOR_TOPLEFT);
		virtual ~ScrollingTextFrame();

		virtual bool handleEvents(const InputHandler &input, const EventHandler &events);
		void update(const InputHandler &input);
		virtual void draw();

		void setScrollBarWidth(int w) { scroll_bar_width = w; }
		void setScrollSpeed(int sp) { scroll_speed = sp; }
		void setScrollBarPadding(int pad) { scroll_bar_padding = pad; }

		void setTextPadding(int pad) { text_padding = pad; }
		void setFont(std::string file);

		void insertString(ALLEGRO_COLOR cl, int flags, std::string str);

		int getTextBottom(void);

	private:

		struct TextObj
		{
			std::string string;
			int flags;
			ALLEGRO_COLOR cl;
			int position;
			int rows;
		};

		Timer timer;

		ResourceHandle<Font> font;

		int font_width;
		int text_height;
		int text_padding;

		int scroll_bar_height;
		int scroll_bar_width;
		int scroll_bar_position;
		int scroll_bar_padding;
		int scroll_speed;

		int position;

		void calculateScrollBarPosition();

		std::vector<TextObj> strings;
	};
}