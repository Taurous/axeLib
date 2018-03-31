#include "axeEngine\GUI\GUIObject.h"

namespace axe
{
	int GUIObject::m_next_id = 0;

	GUIObject::GUIObject(DrawEngine *draw, GUIObject *parent, float x, float y, int offset_x, int offset_y, float width, float height, int anchor)
		:	m_draw(draw), m_parent(parent),
			m_anchor(anchor), m_visible(true),
			m_relative_x(x), m_relative_y(y),
			m_offset_x(offset_x), m_offset_y(offset_y),
			m_width(width), m_height(height)
	{
		if (m_parent)
		{
			m_parent->registerChild(this);
			translateLocalToScreen();
		}
		else
		{
			m_screen_x = x * m_draw->getWindowWidth();
			m_screen_y = y * m_draw->getWindowHeight();
		}

		m_id = ++m_next_id;
	}

	GUIObject::~GUIObject()
	{
		if (m_parent)
		{
			m_parent->unregisterChild(this);
		}
	}

	void GUIObject::translateLocalToScreen(void)
	{
		switch (m_anchor)
		{
		case ANCHOR_TOPLEFT:
			m_screen_x = m_parent->getScreenX() + m_offset_x;
			m_screen_y = m_parent->getScreenY() + m_offset_y;
			break;
		case ANCHOR_TOP:
			m_screen_x = m_parent->getScreenX() + (m_parent->getWidth() / 2) + m_offset_x - (m_width / 2);
			m_screen_y = m_parent->getScreenY() + m_offset_y;
			break;
		case ANCHOR_TOPRIGHT:
			m_screen_x = m_parent->getScreenX() + m_parent->getWidth() + m_offset_x - m_width;
			m_screen_y = m_parent->getScreenY() + m_offset_y;
			break;
		case ANCHOR_RIGHT:
			m_screen_x = m_parent->getScreenX() + m_parent->getWidth() + m_offset_x - m_width;
			m_screen_y = m_parent->getScreenY() + (m_parent->getHeight() / 2) + m_offset_y - (m_height / 2);
			break;
		case ANCHOR_BOTTOMRIGHT:
			m_screen_x = m_parent->getScreenX() + m_parent->getWidth() + m_offset_x - m_width;
			m_screen_y = m_parent->getScreenY() + m_parent->getHeight() + m_offset_y - m_height;
			break;
		case ANCHOR_BOTTOM:
			m_screen_x = m_parent->getScreenX() + (m_parent->getWidth() / 2) + m_offset_x - (m_width / 2);
			m_screen_y = m_parent->getScreenY() + m_parent->getHeight() + m_offset_y - m_height;
			break;
		case ANCHOR_BOTTOMLEFT:
			m_screen_x = m_parent->getScreenX() + m_offset_x;
			m_screen_y = m_parent->getScreenY() + m_parent->getHeight() + m_offset_y - m_height;
			break;
		case ANCHOR_LEFT:
			m_screen_x = m_parent->getScreenX() + m_offset_x;
			m_screen_y = m_parent->getScreenY() + (m_parent->getHeight() / 2) + m_offset_y - (m_height / 2);
			break;
		case ANCHOR_CENTER:
			m_screen_x = m_parent->getScreenX() + (m_parent->getWidth() / 2) + m_offset_x - (m_width / 2);
			m_screen_y = m_parent->getScreenY() + (m_parent->getHeight() / 2) + m_offset_y - (m_height / 2);
			break;
		default:
			break;
		}

		for (GUIObject *it : m_children)
		{
			it->translateLocalToScreen();
		}
	}

	void GUIObject::translateScreenToLocal(void)
	{
		switch (m_anchor)
		{
		case ANCHOR_TOPLEFT:
			m_offset_x = m_screen_x - m_parent->getScreenX();
			m_offset_y = m_screen_y - m_parent->getScreenY();
			break;
		case ANCHOR_TOP:
			m_offset_x = (m_screen_x + (m_width / 2)) - (m_parent->getScreenX() + (m_parent->getWidth() / 2));
			m_offset_y = m_screen_y - m_parent->getScreenY();
			break;
		case ANCHOR_TOPRIGHT:
			m_offset_x = -m_parent->getScreenX() - m_parent->getWidth() + m_screen_x + m_width;
			m_offset_y = m_screen_y - m_parent->getScreenY();
			break;
		case ANCHOR_RIGHT:
			m_offset_x = -m_parent->getScreenX() - m_parent->getWidth() + m_screen_x + m_width;
			m_offset_y = (m_screen_y + (m_height / 2)) - (m_parent->getScreenY() + (m_parent->getHeight() / 2));
			break;
		case ANCHOR_BOTTOMRIGHT:
			m_offset_x = -m_parent->getScreenX() - m_parent->getWidth() + m_screen_x + m_width;
			m_offset_y = -m_parent->getScreenY() - m_parent->getHeight() + m_screen_y + m_height;
			break;
		case ANCHOR_BOTTOM:
			m_offset_x = (m_screen_x + (m_width / 2)) - (m_parent->getScreenX() + (m_parent->getWidth() / 2));
			m_offset_y = -m_parent->getScreenY() - m_parent->getHeight() + m_screen_y + m_height;
			break;
		case ANCHOR_BOTTOMLEFT:
			m_offset_x = m_screen_x - m_parent->getScreenX();
			m_offset_y = -m_parent->getScreenY() - m_parent->getHeight() + m_screen_y + m_height;
			break;
		case ANCHOR_LEFT:
			m_offset_x = m_screen_x - m_parent->getScreenX();
			m_offset_y = (m_screen_y + (m_height / 2)) - (m_parent->getScreenY() + (m_parent->getHeight() / 2));
			break;
		case ANCHOR_CENTER:
			m_offset_x = (m_screen_x + (m_width / 2)) - (m_parent->getScreenX() + (m_parent->getWidth() / 2));
			m_offset_y = (m_screen_y + (m_height / 2)) - (m_parent->getScreenY() + (m_parent->getHeight() / 2));
			break;
		default:
			break;
		}
	}

	void GUIObject::draw()
	{
		if (m_visible)
		{
			for (GUIObject *it : m_children)
			{
				it->draw();
			}
		}
	}
	void GUIObject::moveTo(int x, int y)
	{
		if (!m_parent)
		{
			printf("Attempting to move GUI Object with no parent!\nUIParent cannot be moved,\nor you have an object without a parent.\n");
		}
		else
		{
			m_screen_x = x;
			m_screen_y = y;

			translateScreenToLocal(); // translates given screen coords to local coords

			for (GUIObject *it : m_children)
			{
				it->translateLocalToScreen();
			}
		}
	}
	void GUIObject::moveAdd(int x, int y)
	{
		if (!m_parent)
		{
			printf("Attempting to move GUI Object with no parent!\nUIParent cannot be moved,\nor you have an object without a parent.\n");
		}
		else
		{
			m_offset_x += x;
			m_offset_y += y;
			translateLocalToScreen();
		}
	}
	void GUIObject::registerChild(GUIObject *obj)
	{
		m_children.push_back(obj);
	}
	void GUIObject::unregisterChild(GUIObject *obj)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			if ((*it) == obj)
			{
				m_children.erase(it);
				break;
			}
		}
	}

	void GUIObject::setAbsoluteX(int x)
	{
		if (m_parent)
		{
			m_abs_x = m_parent->getScreenX() + x;
			m_rel_x = float(m_draw->getWindowWidth()) / float(x);
		}
		else
		{
			m_abs_x = x;
		}
	}
	void GUIObject::setAbsoluteY(int y);
	void GUIObject::setRelativeX(float x);
	void GUIObject::setRelativeY(float y);

	void GUIObject::setAbsoluteWidth(int width);
	void GUIObject::setAbsoluteHeight(int height);
	void GUIObject::setRelativeWidth(float width);
	void GUIObject::setRelativeHeight(float height);
}