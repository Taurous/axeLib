#pragma once

#include <allegro5/allegro.h>
#include "GameObject.h"

#include "InputHandler.h"

class SimpleObject : public GameObject
{
public:
	SimpleObject(float x, float y);
	~SimpleObject();

	void handleEvents(axe::InputHandler &input);
	void update(double deltaTime);

	const float &getSpeedX() const { return m_speed_x; }
	const float &getSpeedY() const { return m_speed_y; }

	void setSpeedX(const float &speed) { m_speed_x = speed; }
	void setSpeedY(const float &speed) { m_speed_y = speed; }

private:
	float m_speed_x;
	float m_speed_y;

	unsigned long long m_ticks;
};