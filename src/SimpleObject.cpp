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
	m_world_x = cos(double(m_ticks) / 20.f) * 640 + 640;
	m_world_y = sin(double(m_ticks) / 50.f) * 360 + 360;

	//m_world_x += m_speed_x * deltaTime;
	//m_world_y += m_speed_y * deltaTime;

	m_ticks++;
}