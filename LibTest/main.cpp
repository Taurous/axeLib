#include <allegro5\allegro.h>

#include <axeLib\InputHandler.h>
#include <axeLib\EventHandler.h>
#include <axeLib\DrawEngine.h>
#include <axeLib\StateManager.h>
#include <axeLib\SettingsHandler.h>

#include <axeLib\util\FPS.h>

#include "SimpleState.h"

const int DEFAULT_WIND_WIDTH = 1280;
const int DEFAULT_WIND_HEIGHT = 720;

int main(int argc, char ** argv)
{
	// First create an instance of each system that is needed.
	// DrawEngine depends on EventHandler 

	axe::InputHandler m_input;
	axe::EventHandler m_events(60);
	axe::DrawEngine m_draw;
	axe::StateManager m_states;

	// Create SettingsHandler.
	// Set some default settings, then load the settings and pass a reference into get(). get() returns false if the setting does not exist or cannot otherwise be retrieved.

	axe::SettingsHandler s;

	s.set("width", DEFAULT_WIND_WIDTH);
	s.set("height", DEFAULT_WIND_HEIGHT);

	s.loadSettings(".settings");
	
	int w, h;
	s.get("width", w);
	s.get("height", h);

	// Create Window then register the window for events.

	std::string windName = "axeLib v0.5.0 Test";
	m_draw.createWindow(w, h, windName);
	m_draw.getWindow().registerForEvents(m_events.getEventQueue());

	//Set up paths to resources..

	m_draw.fonts.setPathToResources("res/fonts/");
	m_draw.bitmaps.setPathToResources("res/textures/");

	// Load up the inital state.

	m_states.changeState(std::unique_ptr<axe::AbstractState>(new SimpleState(m_states, m_input, m_events, m_draw)));

	axe::Timer clean_timer;
	clean_timer.start();

	axe::FPSObject fpso;

	m_events.startTimer();
	while (m_states.running())
	{
		if (m_events.handleEvents())
		{
			m_input.getInput(m_events.getEvent());
			m_states.handleEvents();

			if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_CLOSE))
			{
				m_states.quit();
			}
			else if (m_events.eventIs(ALLEGRO_EVENT_TIMER))
			{
				m_states.update();
			}
		}

		if (m_events.eventQueueEmpty())
		{
			m_states.draw();

			m_draw.flipAndClear(al_map_rgb(0, 0, 0));
			fpso.calculateAverageFps();
		}

		if (clean_timer.elapsed().count() / 1000 > 5)
		{
			clean_timer.restart();
			m_states.cleanStates();
		}
	}

	s.set("width", m_draw.getWindowWidth());
	s.set("height", m_draw.getWindowHeight());

	s.saveSettings(".settings");
}