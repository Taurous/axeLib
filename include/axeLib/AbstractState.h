#pragma once

// Include these or use dispatch (Observer/Events/Command?)
#include "axeLib/DrawEngine.h"
#include "axeLib/EventHandler.h"
#include "axeLib/InputHandler.h"

namespace axe
{
	class StateManager;

	class AbstractState
	{
	public:
		AbstractState(StateManager &states, InputHandler &input, EventHandler &events, DrawEngine &draw)
		: m_states(states),
		  m_input(input),
		  m_events(events),
		  m_draw(draw)
		{ }

		virtual ~AbstractState() { }

		virtual void pause() = 0;
		virtual void resume() = 0;

		virtual void handleEvents() = 0;
		virtual void update(unsigned long long deltaTime) = 0;
		virtual void draw() = 0;

	protected:
		StateManager &m_states;
		InputHandler &m_input;
		EventHandler &m_events;
		DrawEngine &m_draw;
	};
}