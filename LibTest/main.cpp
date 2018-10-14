#include <allegro5\allegro.h>

#include <axeLib\InputHandler.h>
#include <axeLib\EventHandler.h>
#include <axeLib\DrawEngine.h>

#include <axeLib\simpleObject.h>

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

	axe::Timer t;

	const double ticksPerSecond = 60.f;

	axe::InputHandler m_input;
	axe::EventHandler m_events(ticksPerSecond);
	axe::DrawEngine m_draw;

	m_draw.createWindow(DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT, "axeLib v0.5.0 Test").registerForEvents(m_events.getEventQueue());

	printResolution(m_draw);

	SimpleObject obj(DEFAULT_WIND_WIDTH * 0.25, DEFAULT_WIND_HEIGHT / 2.f);

	DrawObjectRef objRef(obj, "simple.png"); // replace with call to something like m_draw->register(obj, layer);

	bool running = true;
	bool redraw = false;

	m_draw.flipAndClear(al_map_rgb(0, 0, 0));

	t.start();
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
				redraw = true;
				double delta = t.restart().count() / 1000.f;

				obj.update(delta);
			}
		}

		if (m_events.eventQueueEmpty() && redraw)
		{
			redraw = false;

			objRef.draw(0, 0);

			//m_draw.flipAndClear(al_map_rgb(0, 0, 0));
			al_flip_display();
		}
	}
}