#pragma once

#include <axeLib\StateManager.h>
#include <axeLib/util/Timer.h>

#include <vector>
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
	static const int world_height = 10;
	static const int world_width = 10;

	ALLEGRO_BITMAP *tileset;

	std::array<std::array<int, world_width>, world_height> world;
	std::array<std::array<int, world_width>, world_height> world_col;

	int tile_size;
	int tiles_wide;
	int tiles_high;
};
