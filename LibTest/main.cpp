#include <allegro5\allegro.h>

#include <axeLib\InputHandler.h>
#include <axeLib\EventHandler.h>
#include <axeLib\DrawEngine.h>
#include <axeLib\StateManager.h>

#include <axeLib\simpleObject.h>

#include "SimpleState.h"

const int DEFAULT_WIND_WIDTH = 1280;
const int DEFAULT_WIND_HEIGHT = 720;

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
	axe::StateManager m_states;

	m_draw.createWindow(DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT, "axeLib v0.5.0 Test").registerForEvents(m_events.getEventQueue());

	printResolution(m_draw);

	m_states.changeState(std::unique_ptr<axe::AbstractState>(new SimpleState(m_states, m_input, m_events, m_draw)));

	bool redraw = false;
	m_events.startTimer();
	while (m_states.running())
	{
		if (m_events.handleEvents())
		{
			m_input.getInput(m_events.getEvent());

			m_states.handleEvents();

			if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_CLOSE) || m_input.isKeyPressed(ALLEGRO_KEY_ESCAPE))
			{
				m_states.quit();
			}
			else if (m_events.eventIs(ALLEGRO_EVENT_TIMER))
			{
				redraw = true;
			}
		}

		if (m_events.eventQueueEmpty() && redraw)
		{
			redraw = false;

			m_states.draw();

			m_draw.flipAndClear(al_map_rgb(0, 0, 0));
		}

		m_states.cleanStates();
	}
}