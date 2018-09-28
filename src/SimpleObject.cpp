#pragma once

#include "axeLib/SimpleObject.h"

SimpleObject::SimpleObject(std::string image_name) : DrawObject(image_name)
{

}

SimpleObject::~SimpleObject()
{

}

void SimpleObject::handleEvents(axe::InputHandler &input)
{
	if (input.isKeyPressed(ALLEGRO_KEY_LEFT))
	{
		setWorldX(0);
	}
	else if (input.isKeyPressed(ALLEGRO_KEY_RIGHT))
	{
		setWorldX(256);
	}
}
