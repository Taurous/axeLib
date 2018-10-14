#pragma once

#include "axeLib/SimpleObject.h"
#include <cmath>

SimpleObject::SimpleObject(float x, float y) : GameObject(x, y), m_speed_x(0), m_speed_y(0), m_ticks(0)
{

}

SimpleObject::~SimpleObject()
{

}

void SimpleObject::handleEvents(axe::InputHandler &input)
{

}

void SimpleObject::update(double deltaTime)
{
	/*float scale = 4.f;
	float speed = 20.f;

	float hPeriod = 6.f;
	float vPeriod = 1.f;

	m_world_x = cos(double(m_ticks) / (hPeriod * speed)) * (100*scale*1.5f) + 640;
	m_world_y = sin(double(m_ticks) / (vPeriod * speed)) * (100*scale) + 360;*/

	//m_world_x += m_speed_x * deltaTime;
	//m_world_y += m_speed_y * deltaTime;

	m_world_x = 640 + (m_ticks * 10);
	m_world_y = 360 - (m_ticks * m_ticks);

	m_ticks++;
}