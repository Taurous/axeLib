#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <axeLib/util/Timer.h>

#include <axeLib/InputHandler.h>
#include <axeLib/EventHandler.h>
#include <axeLib/DrawEngine.h>
#include <axeLib/StateManager.h>

#include "EditorState.h"

int main(int argc, char ** argv)
{
	srand(time(NULL));

	// DrawEngine depends on EventHandler 
	const int DEFAULT_WIND_WIDTH = 1000;
	const int DEFAULT_WIND_HEIGHT = 600;
	const double ticksPerSecond = 60.f;

	al_init_font_addon();
	al_init_ttf_addon();

	axe::InputHandler m_input;
	axe::EventHandler m_events(ticksPerSecond);
	axe::DrawEngine m_draw;
	axe::StateManager m_state;

	m_draw.createWindow(DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT, m_events.getEventQueue(), false, ALLEGRO_RESIZABLE).setWindowTitle("axeLib Test");

	m_state.changeState(std::unique_ptr<axe::AbstractState>(new EditorState(m_state, m_input, m_events, m_draw)));

	ALLEGRO_FONT *fps_font = al_load_font("C:/Windows/Fonts/arial.ttf", 18, 0);
	const int avg_fps_count = 30;
	unsigned long long aFps[avg_fps_count];
	int cur_index = 0;
	float avg_fps = 0.f;

	memset(aFps, 0.f, sizeof(unsigned long long) * avg_fps_count);

	bool redraw = false;

	axe::Timer t;
	axe::TimePoint last_time = t.now();
	axe::TimePoint last_frame_time = t.now();
	t.start();

	m_events.startTimer();
	while (m_state.running())
	{
		if (m_events.handleEvents())
		{
			m_draw.handleEvents(m_events.getEvent());

			m_input.getInput(m_events.getEvent());

			m_state.handleEvents();

			if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_CLOSE) || m_input.isKeyPressed(ALLEGRO_KEY_ESCAPE))
			{
				m_state.quit();
			}
			else if (m_events.eventIs(ALLEGRO_EVENT_TIMER))
			{
				unsigned long long deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(t.now() - last_time).count();
				last_time = t.now();

				m_state.update(deltaTime);
				redraw = true;
			}
			else if (m_input.isKeyPressed(ALLEGRO_KEY_SPACE)) m_draw.getWindow().setFullscreen(!m_draw.getWindow().getFullscreen());
		}

		if (m_events.eventQueueEmpty() && redraw)
		{
			redraw = false;

			auto now = t.now();
			auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame_time).count();
			last_frame_time = now;

			aFps[cur_index++] = deltaTime;

			if (cur_index >= avg_fps_count)
			{
				unsigned long long total = 0;
				for (int i = 0; i < avg_fps_count; ++i)
				{
					total += aFps[i];
				}

				avg_fps = float(total) / float(avg_fps_count);

				avg_fps = 1000.f / avg_fps;

				cur_index = 0;
			}

			m_state.draw();
		
			al_draw_textf(fps_font, al_map_rgb(255, 255, 255), m_draw.getWindow().getWidth() - 16, 16, ALLEGRO_ALIGN_RIGHT, "FPS: %.1f", avg_fps);

			axe::flipAndClear(al_map_rgb(0, 0, 0));
		}

		m_state.cleanStates();
	}
}