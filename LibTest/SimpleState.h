#pragma once

#include <axeLib\StateManager.h>
#include <vector>
#include <axeLib/util/Timer.h>
#include <array>

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
	ALLEGRO_BITMAP *screen_buffer;
	int screen_width, screen_height;

	std::array<std::array<bool, 10>, 10> world;

	float camera_x, camera_y;
	float camera_d;
	float fov;
};
