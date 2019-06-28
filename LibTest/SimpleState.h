#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <axeLib\StateManager.h>
#include <axeLib/util/Timer.h>

#include <vector>
#include <array>

#include "Level.h"
#include "TextBox.h"

class SimpleState : public axe::AbstractState
{
public:
	SimpleState(axe::StateManager &states, axe::InputHandler &input, axe::EventHandler &events, axe::DrawEngine &draw);
	~SimpleState();

	virtual void pause();
	virtual void resume();

	virtual void handleEvents();
	virtual void update(unsigned long long deltaTime);
	virtual void draw();

private:
	Tilemap *tilemap;
	World *world;

	int selection;
	int current_layer;
	bool draw_grid;

	int camera_x;
	int camera_y;
	int camera_halfwidth;
	int camera_halfheight;

	ALLEGRO_FONT *font;

	TextBox tbox;
};
