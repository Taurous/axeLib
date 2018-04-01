#include <allegro5\allegro.h>

#include <axeLib\InputHandler.h>
#include <axeLib\EventHandler.h>
#include <axeLib\DrawEngine.h>
#include <axeLib\StateManager.h>
#include <axeLib\SettingsHandler.h>

#include <axeLib\util\FPS.h>

int main(int argc, char ** argv)
{
	axe::InputHandler m_input;
	axe::EventHandler m_events(60);
	axe::DrawEngine m_draw;
	axe::StateManager m_states;

	int windowWidth = 1280;
	int windowHeight = 768;
	std::string windName = "axeLib v0.5.0 Test";

	axe::SettingsHandler s;

	s.set("width", windowWidth);
	s.set("height", windowHeight);

	s.loadSettings(".settings");
	
	s.get("width", windowWidth);
	s.get("height", windowHeight);

	m_draw.createWindow(windowWidth, windowHeight, windName);
	m_draw.getWindow().registerForEvents(m_events.getEventQueue());

	m_draw.fonts.setPathToResources("res/fonts/");
	m_draw.bitmaps.setPathToResources("res/textures/");

	//m_states.changeState(std::unique_ptr<axe::AbstractState>(new SplashState(m_states, m_input, m_events, m_draw)));

	axe::Timer clean_timer;
	clean_timer.start();

	axe::FPSObject fpso;

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
			}
		}

		if (m_events.eventQueueEmpty())
		{
			//m_states.draw();

			m_draw.flipAndClear(al_map_rgb(0, 0, 0));
			fpso.calculateAverageFps();
		}

		if (clean_timer.elapsed().count() / 1000 > 5)
		{
			clean_timer.restart();
			m_draw.bitmaps.removeUnreferencedResources();
			m_draw.fonts.removeUnreferencedResources();
			m_states.cleanStates();
		}
	}

	s.set("width", m_draw.getWindowWidth());
	s.set("height", m_draw.getWindowHeight());

	s.saveSettings(".settings");
}