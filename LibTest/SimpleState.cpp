#include "SimpleState.h"

SimpleState::SimpleState(axe::StateManager & states, axe::InputHandler & input, axe::EventHandler & events, axe::DrawEngine & draw) : AbstractState(states, input, events, draw)
{
	anim.push_back(al_load_bitmap("pixil-frame-0.png"));
	anim.push_back(al_load_bitmap("pixil-frame-1.png"));
	anim.push_back(al_load_bitmap("pixil-frame-2.png"));
	anim.push_back(al_load_bitmap("pixil-frame-3.png"));
	anim.push_back(al_load_bitmap("pixil-frame-4.png"));
	anim.push_back(al_load_bitmap("pixil-frame-5.png"));
	anim.push_back(al_load_bitmap("pixil-frame-6.png"));
	anim.push_back(al_load_bitmap("pixil-frame-7.png"));

	back = al_load_bitmap("back.png");

	frame = anim.begin();

	x = m_draw.getWindow().getWidth() / 2;
	y = m_draw.getWindow().getHeight() / 2;

	speed_x = 0;
	speed_y = 0;

	facing_right = false;

	t.start();
}

SimpleState::~SimpleState()
{
	for (auto e : anim)
	{
		al_destroy_bitmap(e);
	}
	anim.clear();

	al_destroy_bitmap(back);
}

void SimpleState::pause() { }
void SimpleState::resume() { }

void SimpleState::handleEvents()
{
	if (m_input.isKeyDown(ALLEGRO_KEY_A))
	{
		speed_x = -200;
	}
	else if (m_input.isKeyDown(ALLEGRO_KEY_D))
	{
		speed_x = 200;
	}
	else
	{
		speed_x = 0;
	}

	if (m_input.isKeyDown(ALLEGRO_KEY_W))
	{
		speed_y = -200;
	}
	else if (m_input.isKeyDown(ALLEGRO_KEY_S))
	{
		speed_y = 200;
	}
	else
	{
		speed_y = 0;
	}

	if (m_input.getMouseX() > x) facing_right = true;
	else facing_right = false;
	if (m_input.getMouseY() > y) facing_up = false;
	else facing_up = true;
}
void SimpleState::update(unsigned long long deltaTime)
{
	if (t.elapsed().count() > 200)
	{
		t.restart();
		frame++;
		if (frame == anim.end())
		{
			frame = anim.begin();
		}
	}

	printf("deltaTime = %llu\n", deltaTime);

	x += speed_x * (deltaTime / 1000.f);
	y += speed_y * (deltaTime / 1000.f);
}
void SimpleState::draw()
{
	ALLEGRO_BITMAP *b;

	b = *frame;

	if (facing_up) b = back;

	int flags = 0;
	if (facing_right) flags = ALLEGRO_FLIP_HORIZONTAL;
	al_draw_scaled_bitmap(b, 0, 0, 32, 32, x, y, 96, 96, flags);
}