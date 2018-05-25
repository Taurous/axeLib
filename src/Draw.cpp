#include "axeLib\DrawEngine.h"

namespace axe
{
	typedef std::vector<std::string>::iterator it;

	void axe::DrawEngine::drawText(ResourceHandle<Font> font, ALLEGRO_COLOR cl, float x, float y, int flags, std::string text)
	{
		if (!font) return;

		ALLEGRO_FONT *f = fonts[font]->getData();

		if (flags & TEXT_JITTER)
		{
			float char_width = 0;

			for (unsigned int i = 0; i < text.size(); ++i)
			{
				char c = text[i];
				float x_jitter = float(rand() % 101 - 50) / 30.f;
				float y_jitter = float(rand() % 101 - 50) / 30.f;
				if (f)
				{
					al_draw_text(f, cl, x + char_width + x_jitter, y + y_jitter, flags, &c);
					char_width += al_get_text_width(f, &c);
				}
			}
		}
		else al_draw_textf(f, cl, x, y, flags, "%s", text.c_str());
	}

	void axe::DrawEngine::drawTextF(ResourceHandle<Font> font, ALLEGRO_COLOR cl, float x, float y, int flags, std::string format, ...)
	{
		char buffer[256];

		va_list arg;
		va_start(arg, format);
		vsnprintf_s(buffer, 256, format.c_str(), arg);
		va_end(arg);

		drawText(font, cl, x, y, flags, buffer);
	}

	void DrawEngine::drawTextWithCursor(ResourceHandle<Font> font, ALLEGRO_COLOR cl, float x, float y, int flags, m_secs speed, unsigned int string_length, std::string text)
	{
		if (!font) return;

		drawText(font, cl, x, y, flags, text);

		if (t_cursorFlash.elapsed() >= speed * 2)
		{
			t_cursorFlash.start();
		}
		else if (t_cursorFlash.elapsed() > speed && text.size() < string_length)
		{
			drawText(font, cl, x + fonts[font]->getStringWidth(text), y, 0, "_");
		}
	}

	void DrawEngine::drawTextNewLine(ResourceHandle<Font> font, ALLEGRO_COLOR cl, float x, float y, int spacing, int flags, std::string str)
	{
		if (!font) return;

		typedef std::vector<std::string>::iterator it;
		std::vector<std::string> strings;

		int beg = 0;
		int len = 0;

		for (unsigned int i = 0; i <= str.size(); i++, len++)
		{
			if (str[i] == '\n' || i == str.size())
			{
				std::string new_str = str.substr(beg, len);
				strings.push_back(new_str);

				beg = i + 1;
				len = -1;
			}
		}

		int h = fonts[font]->getFontHeight();

		int j = 0;
		for (it i = strings.begin(); i != strings.end(); ++i)
		{
			drawText(font, cl, x, y + (j*(h + spacing)), flags, (*i));
			//al_draw_text(font, cl, x, y + (j*(h + spacing)), flags, (*i).c_str());
			j++;
		}
	}

	void DrawEngine::drawTextNewLineWithCursor(ResourceHandle<Font> font, ALLEGRO_COLOR cl, float x, float y, int spacing, int flags, m_secs speed, unsigned int string_length, std::string str)
	{
		if (!font) return;

		std::vector<std::string> strings;

		int beg = 0;
		int len = 0;

		for (unsigned int i = 0; i <= str.size(); i++, len++)
		{
			if (str[i] == '\n' || i == str.size())
			{
				std::string new_str = str.substr(beg, len);
				strings.push_back(new_str);

				beg = i + 1;
				len = -1;
			}
		}

		int h = fonts[font]->getFontHeight();

		int size = 0;
		int j = 0;
		it k = --strings.end();
		it i;
		for (i = strings.begin(); i != k; ++i)
		{
			drawText(font, cl, x, y + (j*(h + spacing)), flags, (*i));
			size += (*i).size();
			j++;
		}
		if (str.size() >= string_length)
		{
			drawText(font, cl, x, y + (j*(h + spacing)), flags, (*i));
		}
		else
		{
			drawTextWithCursor(font, cl, x, y + (j*(h + spacing)), flags, speed, string_length, (*i).c_str());
		}
	}

	void DrawEngine::drawBitmap(ResourceHandle<Bitmap> bmp, int x, int y, int flags)
	{
		al_draw_bitmap(bitmaps[bmp]->getData(), x, y, flags);
	}

	void DrawEngine::drawScaledBitmap(ResourceHandle<Bitmap> bmp, int x, int y, float x_scale, float y_scale, int flags)
	{
		int w = bitmaps[bmp]->getWidth();
		int h = bitmaps[bmp]->getHeight();

		al_draw_scaled_bitmap(bitmaps[bmp]->getData(), 0, 0, w, h, x, y, float(w) * x_scale, float(h) * y_scale, flags);
	}

	void DrawEngine::drawScaledBitmap(ResourceHandle<Bitmap> bmp, int x, int y, int width, int height, int flags)
	{
		int w = bitmaps[bmp]->getWidth();
		int h = bitmaps[bmp]->getHeight();

		al_draw_scaled_bitmap(bitmaps[bmp]->getData(), 0, 0, w, h, x, y, width, height, flags);
	}

	void DrawEngine::drawTintedBitmap(ResourceHandle<Bitmap> bmp, ALLEGRO_COLOR cl, int x, int y, int flags)
	{
		al_draw_tinted_bitmap(bitmaps[bmp]->getData(), cl, x, y, flags);
	}

	void DrawEngine::flipAndClear(ALLEGRO_COLOR cl)
	{
		al_flip_display();
		al_clear_to_color(cl);
	}
}