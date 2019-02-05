#pragma once

#include <axeLib\StateManager.h>
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
	virtual void update(unsigned long long deltaTime);
	virtual void draw();

private:
	ALLEGRO_BITMAP *back;
	std::vector<ALLEGRO_BITMAP *> anim;
	std::vector<ALLEGRO_BITMAP *>::iterator frame;
	float x, y;
	float speed_x, speed_y;

	bool facing_right;
	bool facing_up;

	axe::Timer t;
};
