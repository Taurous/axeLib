#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <axeLib/InputHandler.h>
#include <axeLib/EventHandler.h>
#include <axeLib/DrawEngine.h>
#include <axeLib/StateManager.h>

#include "SimpleState.h"

int main(int argc, char ** argv)
{
	// DrawEngine depends on EventHandler 
	const int DEFAULT_WIND_WIDTH = 1000;
	const int DEFAULT_WIND_HEIGHT = 400;
	const double ticksPerSecond = 60.f;

	int player_x = DEFAULT_WIND_WIDTH / 2;
	int player_y = DEFAULT_WIND_HEIGHT / 2;
	float light_size = 300.f;

	al_init_font_addon();
	al_init_ttf_addon();

	axe::InputHandler m_input;
	axe::EventHandler m_events(ticksPerSecond);
	axe::DrawEngine m_draw;
	axe::StateManager m_state;

	m_draw.createWindow(DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT, m_events.getEventQueue(), false, ALLEGRO_RESIZABLE | ALLEGRO_MAXIMIZED).setWindowTitle("axeLib Test");

	player_x = m_draw.getWindow().getWidth() / 2;
	player_y = m_draw.getWindow().getHeight() / 2;

	m_state.changeState(std::unique_ptr<axe::AbstractState>(new SimpleState(m_state, m_input, m_events, m_draw)));

	/*ALLEGRO_SHADER *test_shader = nullptr;
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

	al_use_shader(NULL);
	bool using_shader = false;*/
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
			else if (m_input.isKeyPressed(ALLEGRO_KEY_SPACE))
			{
				//using_shader = !using_shader;
			}
			else if (m_input.isKeyPressed(ALLEGRO_KEY_W))
			{
				light_size += 25.f;
			}
			else if (m_input.isKeyPressed(ALLEGRO_KEY_S))
			{
				light_size -= 25.f;
				if (light_size < 25.f) light_size = 25.f;
			}
			
			if (m_input.isKeyDown(ALLEGRO_KEY_RIGHT))
			{
				player_x += 22;
				if (player_x > m_draw.getWindow().getWidth())
					player_x = m_draw.getWindow().getWidth();
			}
			else if (m_input.isKeyDown(ALLEGRO_KEY_LEFT))
			{
				player_x -= 22;
				if (player_x < 0)
					player_x = 0;
			}
			
			if (m_input.isKeyDown(ALLEGRO_KEY_UP))
			{
				player_y -= 22;
				if (player_y < 0)
					player_y = 0;
			}
			else if (m_input.isKeyDown(ALLEGRO_KEY_DOWN))
			{
				player_y += 22;
				if (player_y > m_draw.getWindow().getHeight())
					player_y = m_draw.getWindow().getHeight();
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

			/*float width = m_draw.getWindow().getWidth();
			float height = m_draw.getWindow().getHeight();

			float shader_vals[2];
			shader_vals[0] = player_x;
			shader_vals[1] = player_y;

			al_set_shader_float_vector("mouse_axes", 2, shader_vals, 1);

			shader_vals[0] = width;
			shader_vals[1] = height;

			al_set_shader_float_vector("screen_size", 2, shader_vals, 1);
			al_set_shader_float("light_size", light_size);
			al_set_shader_float("fade_size", 100.f);*/

			m_state.draw();

			//al_use_shader(NULL);
			//al_draw_circle(player_x, player_y, light_size, al_map_rgb(255, 0, 255), 1);
			//al_draw_circle(player_x, player_y, light_size+100, al_map_rgb(255, 0, 255), 1);
			
			//if (using_shader) al_use_shader(test_shader);
			//else al_use_shader(nullptr);

			axe::flipAndClear(al_map_rgb(37, 19, 26));
		}

		m_state.cleanStates();
	}

	//al_destroy_shader(test_shader);
}