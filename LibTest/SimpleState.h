#pragma once

#include <axeLib\AbstractState.h>

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
	axe::Timer t;

	int m_block_size;
	int m_grid_width;
	int m_grid_height;

	int m_player_x;
	int m_player_y;
	int m_dir_x;
	int m_dir_y;
	int m_t_dir_x;
	int m_t_dir_y;
};
