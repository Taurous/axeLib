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

}
void SimpleState::update()
{
	
}
void SimpleState::draw()
{

}