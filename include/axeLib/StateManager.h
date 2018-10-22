#pragma once

#include <inttypes.h>
#include <vector>
#include <memory>

#include "util\Logger.h"

#include "DrawEngine.h"
#include "InputHandler.h"
#include "EventHandler.h"

#include "AbstractState.h"

/*
*	Note to self, need to remove the ability
*	to pop states until the list is empty.

What the hell did I mean by this?
*/

namespace axe
{
	enum
	{
		STATE_CHANGE,
		STATE_POP,
		STATE_PUSH
	};

	class StateManager
	{
	public:
		StateManager();

		StateManager(const StateManager &other) = delete;
		StateManager &operator=(const StateManager &other) = delete;

		StateManager(StateManager &&other) = delete;
		StateManager &operator=(StateManager &&other) = delete;

		~StateManager();

		void changeState(std::unique_ptr<AbstractState> state);
		void pushState(std::unique_ptr<AbstractState> state);
		void popState();
		void popState(int flag, std::unique_ptr<AbstractState> state);

		void cleanStates();

		void handleEvents();
		void update();
		void draw();

		void setDrawAll(bool flag) { m_draw_all = flag; }

		bool running() const { return m_running; }
		void quit() { m_running = false; }

	private:
		bool m_running;
		bool m_res;
		bool m_draw_all;

		std::vector<std::unique_ptr<AbstractState>> m_states;
		std::vector<std::unique_ptr<AbstractState>> m_dead_states;
	};

}