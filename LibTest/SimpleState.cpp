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
	if (m_input.isMousePressed(axe::MOUSE_LEFT))
	{
		c.addPoint(m_input.getMouseX(), m_input.getMouseY());
	}
	else if (m_input.isMousePressed(axe::MOUST_RIGHT))
	{
		c.clearPoints();
	}
}
void SimpleState::update()
{
	
}
void SimpleState::draw()
{
	c.draw();
}