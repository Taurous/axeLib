#include <allegro5\allegro.h>

#include <axeLib\InputHandler.h>
#include <axeLib\EventHandler.h>
#include <axeLib\DrawEngine.h>
#include <axeLib\StateManager.h>

#include "SimpleState.h"

#include <vector>

const int DEFAULT_WIND_WIDTH = 800;
const int DEFAULT_WIND_HEIGHT = 600;

void printResolution(axe::DrawEngine &draw)
{
	axe::log(axe::LOGGER_MESSAGE, "Screen Resolution is: %ix%i\n",
		draw.getWindow().getScreenWidth(),
		draw.getWindow().getScreenHeight()
	);

	axe::log(axe::LOGGER_MESSAGE, "Window Resolution is: %ix%i\n",
		draw.getWindow().getWindowWidth(),
		draw.getWindow().getWindowHeight()
	);
}

int main(int argc, char ** argv)
{
	// DrawEngine depends on EventHandler 

	const double ticksPerSecond = 60.f;

	axe::InputHandler m_input;
	axe::EventHandler m_events(ticksPerSecond);
	axe::DrawEngine m_draw;
	axe::StateManager m_state;

	m_draw.createWindow(DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT, "axeLib Test").registerForEvents(m_events.getEventQueue());

	printResolution(m_draw);

	m_state.changeState(std::unique_ptr<axe::AbstractState>(new SimpleState(m_state, m_input, m_events, m_draw)));

	bool redraw = false;

	m_events.startTimer();
	while (m_state.running())
	{
		if (m_events.handleEvents())
		{
			m_input.getInput(m_events.getEvent());

			m_state.handleEvents();

			if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_CLOSE) || m_input.isKeyPressed(ALLEGRO_KEY_ESCAPE))
			{
				m_state.quit();
			}
			else if (m_events.eventIs(ALLEGRO_EVENT_TIMER))
			{
				m_state.update();
				redraw = true;
			}
		}

		if (m_events.eventQueueEmpty() && redraw)
		{
			redraw = false;

			m_state.draw();

			m_draw.flipAndClear(al_map_rgb(0, 0, 0));
		}

		m_state.cleanStates();
	}
}