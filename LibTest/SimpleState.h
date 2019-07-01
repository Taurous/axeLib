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

struct Camera
{
	float x;
	float y;
	float halfwidth;
	float halfheight;
};

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
	std::shared_ptr<Level> level;

	int selection;
	int current_layer;
	bool draw_grid;

	Camera cam;

	ALLEGRO_FONT *font;
	ALLEGRO_FONT *font_small;

	TextBox tbox;

	std::vector<std::unique_ptr<Command>> vCommands_undo;
	std::vector <std::unique_ptr<Command>> vCommands_redo;
};
