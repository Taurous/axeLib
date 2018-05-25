#include "SimpleState.h"

SimpleState::SimpleState(axe::StateManager & states, axe::InputHandler & input, axe::EventHandler & events, axe::DrawEngine & draw) : AbstractState(states, input, events, draw)
{

}

SimpleState::~SimpleState()
{

}

void SimpleState::pause() { }
void SimpleState::resume() { }

void SimpleState::handleEvents()
{
	if (m_input.isMousePressed(axe::MOUSE_LEFT, axe::MOD_CTRL | axe::MOD_ALT))
	{
		axe::log(axe::LOGGER_MESSAGE, "Left Mouse + Control Pressed!\n");
	}
}
void SimpleState::update()
{
	
}
void SimpleState::draw()
{

}