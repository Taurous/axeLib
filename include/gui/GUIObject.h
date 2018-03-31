#pragma once

#include "DrawEngine.h"
#include <list>

namespace axe
{
	enum 
	{
		ANCHOR_TOPLEFT,
		ANCHOR_TOP,
		ANCHOR_TOPRIGHT,
		ANCHOR_RIGHT,
		ANCHOR_BOTTOMRIGHT,
		ANCHOR_BOTTOM,
		ANCHOR_BOTTOMLEFT,
		ANCHOR_LEFT,
		ANCHOR_CENTER
	};

	class GUIObject
	{
	protected:
		DrawEngine *m_draw;

		float m_relative_x, m_relative_y;
		short m_offset_x, m_offset_y;
		float m_width, m_height;

		bool m_visible;
		int m_anchor;
		int m_id;

		static int m_next_id;

		GUIObject *m_parent;
		std::list<GUIObject*> m_children;

	private:
		void registerChild(GUIObject *obj);
		void unregisterChild(GUIObject *obj);
		void translateLocalToScreen(void);
		void translateScreenToLocal(void);

	public:
		GUIObject(DrawEngine *draw, GUIObject *parent, float x, float y, int offset_x, int offset_y, float width, float height, int anchor = ANCHOR_TOPLEFT);
		GUIObject(const GUIObject&) = delete;
		void operator=(GUIObject const&) = delete;;
		virtual ~GUIObject();

		int getScreenX(void) { return m_relative_x * m_draw->getWindowWidth(); }
		int getScreenY(void) { return m_relative_y * m_draw->getWindowHeight(); }

		int getWidth(void) { return m_width * m_draw->getWindowWidth(); }
		int getHeight(void) { return m_height * m_draw->getWindowHeight(); }

		bool isVisible(void) { return m_visible; }
		void setVisible(bool visible) { m_visible = visible; }

		virtual void draw();

		void moveAdd(int x, int y);
		void moveTo(int x, int y);

		int getID(void) { return m_id; }
	};
}