#include <allegro5/allegro.h>

#include <axeLib/InputHandler.h>
#include <axeLib/EventHandler.h>
#include <axeLib/DrawEngine.h>
#include <axeLib/StateManager.h>

#include "SimpleState.h"

int main(int argc, char ** argv)
{
	// DrawEngine depends on EventHandler 

	const int DEFAULT_WIND_WIDTH = 600;
	const int DEFAULT_WIND_HEIGHT = 400;
	const double ticksPerSecond = 60.f;

	axe::InputHandler m_input;
	axe::EventHandler m_events(ticksPerSecond);
	axe::DrawEngine m_draw;
	axe::StateManager m_state;

	m_draw.createWindow(DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT, m_events.getEventQueue(), false, ALLEGRO_RESIZABLE).setWindowTitle("axeLib Test");

	m_state.changeState(std::unique_ptr<axe::AbstractState>(new SimpleState(m_state, m_input, m_events, m_draw)));


	ALLEGRO_SHADER *test_shader = nullptr;
	test_shader = al_create_shader(ALLEGRO_SHADER_AUTO);

	if (!al_attach_shader_source_file(test_shader, ALLEGRO_VERTEX_SHADER, "vert.hlsl"))
	{
		printf("Vertex Shader Error:\n\t%s\n", al_get_shader_log(test_shader));
	}
	else if (!al_build_shader(test_shader))
	{
		printf("Build Shader Error:\n\t%s\n", al_get_shader_log(test_shader));
	}
	else if (!al_use_shader(test_shader))
	{
		printf("Failed to use shader!\n");
	}

	if (!al_attach_shader_source_file(test_shader, ALLEGRO_PIXEL_SHADER, "shader.hlsl"))
	{
		printf("Pixel Shader Error:\n\t%s\n", al_get_shader_log(test_shader));
	}
	else if (!al_build_shader(test_shader))
	{
		printf("Build Shader Error:\n\t%s\n", al_get_shader_log(test_shader));
	}
	else if (!al_use_shader(test_shader))
	{
		printf("Failed to use shader!\n");
	}

	bool redraw = false;

	axe::Timer t;
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
				m_state.update((unsigned long long)t.restart().count());
				redraw = true;
			}
			/*else if (m_input.isKeyPressed(ALLEGRO_KEY_SPACE))
			{
				m_draw.getWindow().setFullscreen(!m_draw.getWindow().getFullscreen());

				//ALLEGRO_DISPLAY *disp = m_draw.getWindow().getAllegroDisplay();

				//al_toggle_display_flag(disp, ALLEGRO_FULLSCREEN_WINDOW, !m_draw.getWindow().getFullscreen());
			}*/
		}

		if (m_events.eventQueueEmpty() && redraw)
		{
			redraw = false;

			float mouse_x = m_input.getMouseX();

			al_set_shader_float("mouse_x", mouse_x);

			m_state.draw();

			axe::flipAndClear(al_map_rgb(64, 64, 64));
		}

		m_state.cleanStates();
	}

	al_destroy_shader(test_shader);
}