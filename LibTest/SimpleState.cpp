#include "SimpleState.h"

SimpleState::SimpleState(axe::StateManager & states, axe::InputHandler & input, axe::EventHandler & events, axe::DrawEngine & draw)
	: m_block_size(20), m_dir_x(0), m_dir_y(0), m_t_dir_x(0), m_t_dir_y(0), AbstractState(states, input, events, draw)
{
	m_grid_width = m_draw.getWindow().getWindowWidth() / m_block_size;
	m_grid_height = m_draw.getWindow().getWindowHeight() / m_block_size;

	m_player_x = m_grid_width / 2;
	m_player_y = m_grid_height / 2;

	t.start();
}

SimpleState::~SimpleState()
{

}

void SimpleState::pause() { }
void SimpleState::resume() { }

void SimpleState::handleEvents()
{
	if (m_input.isKeyPressed(ALLEGRO_KEY_LEFT))
	{
		m_t_dir_x = -1;
		m_t_dir_y = 0;
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_RIGHT))
	{
		m_t_dir_x = 1;
		m_t_dir_y = 0;
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_UP))
	{
		m_t_dir_x = 0;
		m_t_dir_y = -1;
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_DOWN))
	{
		m_t_dir_x = 0;
		m_t_dir_y = 1;
	}
}
void SimpleState::update()
{
	if (t.elapsed().count() < 100) return;
	else
	{
		t.restart();
		m_dir_x = m_t_dir_x;
		m_dir_y = m_t_dir_y;
	}

	m_player_x += m_dir_x;
	m_player_y += m_dir_y;

	if (m_player_x < 0) m_player_x = m_grid_width - 1;
	else if (m_player_x >= m_grid_width) m_player_x = 0;

	if (m_player_y < 0) m_player_y = m_grid_height - 1;
	else if (m_player_y >= m_grid_height) m_player_y = 0;
}
void SimpleState::draw()
{
	double delta = double(t.elapsed().count()) / 100.f;

	double x_off = delta * m_dir_x;
	double y_off = delta * m_dir_y;

	printf("delta: %.4f\n", delta);

	al_draw_filled_rectangle(double(m_player_x*m_block_size) + (x_off * m_block_size),
							 double(m_player_y*m_block_size) + (y_off * m_block_size),
							 double(m_player_x*m_block_size + m_block_size) + (x_off * m_block_size),
							 double(m_player_y*m_block_size + m_block_size) + (y_off * m_block_size),
							 al_map_rgb(255, 255, 255));
}