#pragma once

#include <axeLib\AbstractState.h>

#include "BezierCurve.h"

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
	BezierCurve c;
};
