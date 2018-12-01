#pragma once

#include <axeLib\AbstractState.h>
#include <vector>
#include <axeLib/util/Timer.h>

class SimpleState : public axe::AbstractState
{
public:
	SimpleState(axe::StateManager &states, axe::InputHandler &input, axe::EventHandler &events, axe::DrawEngine &draw);
	~SimpleState();

	virtual void pause();
	virtual void resume();

	virtual void handleEvents();
	virtual void update();
	virtual void draw();

private:
	std::vector<ALLEGRO_BITMAP *> anim;
	std::vector<ALLEGRO_BITMAP *>::iterator frame;
	int x, y;
	int speed_x, speed_y;

	bool facing_right;

	axe::Timer t;
};
