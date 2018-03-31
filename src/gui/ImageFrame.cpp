#include "axeEngine\GUI\ImageFrame.h"

namespace axe
{
	ImageFrame::ImageFrame(DrawEngine *draw, GUIObject * parent, int x, int y, int width, int height, const std::string &filename, int anchor)
		: Frame(draw, parent, x, y, width, height, anchor)
	{
		bitmap = m_draw->bitmaps.getResource(filename);
	}

	ImageFrame::~ImageFrame()
	{

	}

	void ImageFrame::draw()
	{
		if (m_visible)
		{
			m_draw->drawScaledBitmap(bitmap, m_screen_x, m_screen_y, m_width, m_height, 0);
			al_draw_rectangle(m_screen_x, m_screen_y, m_screen_x + m_width, m_screen_y + m_height, m_border, 1);

			for (GUIObject *child : m_children)
			{
				child->draw();
			}
		}
	}
}