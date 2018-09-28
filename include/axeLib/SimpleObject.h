#pragma once

#include <allegro5/allegro.h>
#include "GameObject.h"

#include "InputHandler.h"

class SimpleObject : public DrawObject
{
public:
	SimpleObject(std::string image_name);
	~SimpleObject();

	void handleEvents(axe::InputHandler &input);

private:
	//Stuff
	
};