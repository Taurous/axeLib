#include <allegro5\allegro.h>

#include <axeLib\InputHandler.h>
#include <axeLib\EventHandler.h>
#include <axeLib\DrawEngine.h>

#include <axeLib/SimpleObject.h>

const int DEFAULT_WIND_WIDTH = 1280;
const int DEFAULT_WIND_HEIGHT = 720;

int main(int argc, char ** argv)
{
	// DrawEngine depends on EventHandler 

	axe::InputHandler m_input;
	axe::EventHandler m_events(60);
	axe::DrawEngine m_draw;

	m_draw.createWindow(DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT, "axeLib v0.5.0 Test").registerForEvents(m_events.getEventQueue());

	axe::log(axe::LOGGER_MESSAGE, "Screen Resolution is: %ix%i\n",
		m_draw.getWindow().getScreenWidth(),
		m_draw.getWindow().getScreenHeight()
	);

	axe::log(axe::LOGGER_MESSAGE, "Window Resolution is: %ix%i\n",
		m_draw.getWindow().getWindowWidth(),
		m_draw.getWindow().getWindowHeight()
	);

	SimpleObject obj("simple.png");

	DrawObjectRef objRef(obj); // replace with call to something like m_draw->register(obj, layer);

	obj.setWorldX(DEFAULT_WIND_WIDTH / 2);
	obj.setWorldY(DEFAULT_WIND_HEIGHT / 2);

	bool running = true;

	m_events.startTimer();
	while (running)
	{
		if (m_events.handleEvents())
		{
			m_input.getInput(m_events.getEvent());

			obj.handleEvents(m_input);

			if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_CLOSE) || m_input.isKeyPressed(ALLEGRO_KEY_ESCAPE))
			{
				running = false;
			}
			else if (m_events.eventIs(ALLEGRO_EVENT_TIMER))
			{
				//tick
			}
		}

		if (m_events.eventQueueEmpty())
		{
			objRef.draw(); // replace with something like m_draw->drawObjects();

			m_draw.flipAndClear(al_map_rgb(0, 0, 0));
		}
	}
}