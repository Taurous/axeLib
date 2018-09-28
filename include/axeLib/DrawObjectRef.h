#pragma once
#include "GameObject.h"

#include <allegro5/allegro.h>

class DrawObjectRef
{
public:
	DrawObjectRef(DrawObject &ref) : m_ref(&ref), bmp(nullptr)
	{
		bmp = al_load_bitmap(m_ref->getImageName().c_str());
	}
	~DrawObjectRef()
	{
		if (bmp) al_destroy_bitmap(bmp);
		bmp = nullptr;
	}

	void draw()
	{
		if (bmp) al_draw_bitmap(bmp, m_ref->getWorldX(), m_ref->getWorldY(), 0);
	}

private:
	DrawObject *m_ref;

	ALLEGRO_BITMAP *bmp;
};