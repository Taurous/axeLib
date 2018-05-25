#include "axeLib\StateManager.h"

using namespace axe;

StateManager::StateManager() : m_draw_all(false), m_running(true), m_res(false) {}

axe::StateManager::~StateManager()
{
	cleanStates();
	while (!m_states.empty()) {
		m_states.pop_back();
	}
}

void StateManager::changeState(std::unique_ptr<AbstractState> state)
{
	popState();
	pushState(std::move(state));
}

void StateManager::pushState(std::unique_ptr<AbstractState> state)
{
	if (!m_states.empty()) {
		m_states.back().get()->pause();
	}

	m_states.push_back(std::move(state));
}

void StateManager::popState()
{
	if (!m_states.empty()) {
		m_dead_states.push_back(std::move(m_states.back()));
		m_states.pop_back();
		if (!m_states.empty()) m_states.back().get()->resume();
	}
}
void StateManager::popState(int _FLAG, std::unique_ptr<AbstractState> state)
{
	if (!m_states.empty()) {
		m_dead_states.push_back(std::move(m_states.back()));
		m_states.pop_back();

		switch (_FLAG)
		{
		case STATE_CHANGE:
			changeState(std::move(state));
			break;
		case STATE_PUSH:
			pushState(std::move(state));
			break;
		case STATE_POP:
			popState();
			break;
		default:
			axe::log(LOGGER_ERROR, "Incorrect flag pass to popState()! Destroying state.\n");
			state.release();
			break;
		}
	}
}

void StateManager::handleEvents()
{
	m_states.back()->handleEvents();
}

void StateManager::update()
{
	m_states.back()->update();
}

void StateManager::draw()
{
	if (m_draw_all)
	{
		for (auto const &ss : m_states) //Draw All!
		{
			ss.get()->draw();
		}
	}
	else m_states.back()->draw(); // Draw Back!
}

void StateManager::cleanStates()
{
	while (!m_dead_states.empty())
	{
		m_dead_states.pop_back();
	}
}