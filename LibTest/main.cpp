#include <allegro5\allegro.h>

#include "InputHandler.h"
#include "EventHandler.h"
#include "Window.h"
#include "DrawEngine.h"
#include "StateManager.h"
#include "SettingsHandler.h"

int main(int argc, char ** argv)
{
	al_init();

	axe::InputHandler m_input;
	axe::EventHandler m_events(60);
	axe::DrawEngine m_draw;
	axe::StateManager m_states;

	int windowWidth = 1280;
	int windowHeight = 768;

	axe::SettingsHandler s;

	s.set("width", windowWidth);
	s.set("height", windowHeight);

	s.loadSettings(".settings");

	s.get("width", windowWidth);
	s.get("height", windowHeight);

	m_draw.createWindow(windowWidth, windowHeight, "axeLib v0.0.1 test");
	m_draw.getWindow().registerForEvents(m_events.getEventQueue());

	m_draw.fonts.setPathToResources("res/fonts/");
	m_draw.bitmaps.setPathToResources("res/textures/");

	bool redraw = true;

	//m_states.changeState(std::unique_ptr<axe::AbstractState>(new SplashState(m_states, m_input, m_events, m_draw)));

	m_events.startTimer();
	while (m_states.running())
	{
		if (m_events.handleEvents())
		{
			m_input.getInput(m_events.getEvent());
			//m_states.handleEvents();

			if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_CLOSE))
			{
				m_states.quit();
			}
			else if (m_events.eventIs(ALLEGRO_EVENT_TIMER))
			{
				//m_states.update();
				m_draw.bitmaps.removeUnreferencedResources();
				m_draw.fonts.removeUnreferencedResources();
				redraw = true;
			}
		}

		if (m_events.eventQueueEmpty() && redraw)
		{
			//m_states.draw();

			m_draw.flipAndClear(al_map_rgb(0, 0, 0));
			redraw = false;
		}

		m_states.cleanStates();
	}

	s.set("width", m_draw.getWindowWidth());
	s.set("height", m_draw.getWindowHeight());

	s.saveSettings(".settings");
}