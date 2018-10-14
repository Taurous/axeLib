#pragma once
#include "GameObject.h"

#include <allegro5/allegro.h>

class DrawObjectRef
{
public:
	DrawObjectRef(GameObject &ref, std::string image_name) : m_ref(&ref), m_bmp(nullptr), m_bitmap_width(1), m_bitmap_height(1)
	{
		m_bmp = al_load_bitmap(image_name.c_str());

		if (m_bmp)
		{
			m_bitmap_width = al_get_bitmap_width(m_bmp);
			m_bitmap_height = al_get_bitmap_height(m_bmp);
		}
	}
	virtual	~DrawObjectRef()
	{
		if (m_bmp) al_destroy_bitmap(m_bmp);
		m_bmp = nullptr;
	}

	virtual void draw(int xOff, int yOff)
	{
		float cur_x = m_ref->getWorldX();
		float cur_y = m_ref->getWorldY();

		//if (m_bmp) al_draw_bitmap(m_bmp, cur_x - m_bitmap_width / 2, cur_y - m_bitmap_width / 2, 0);
		al_draw_filled_circle(cur_x+xOff, cur_y+yOff, 2, al_map_rgb(255, 0, 255));
	}

private:
	GameObject *m_ref;
	ALLEGRO_BITMAP *m_bmp;

	int m_bitmap_width;
	int m_bitmap_height;
};