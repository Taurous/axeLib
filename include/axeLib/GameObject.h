#pragma once

#include <string>

class GameObject
{
public:
	GameObject(float x, float y) : m_world_x(x), m_world_y(y) { }
	~GameObject() { }

	const float &getWorldX() const { return m_world_x; }
	const float &getWorldY() const { return m_world_y; }

	void setWorldX(const float &x) { m_world_x = x; }
	void setWorldY(const float &y) { m_world_y = y; }

protected:
	float m_world_x;
	float m_world_y;
};