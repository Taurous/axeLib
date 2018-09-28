#pragma once

#include <string>

class GameObject
{
public:
	GameObject() : m_world_x(0), m_world_y(0) { }
	~GameObject() { }

	const double &getWorldX() const { return m_world_x; }
	const double &getWorldY() const { return m_world_y; }

	void setWorldX(const double &x) { m_world_x = x; }
	void setWorldY(const double &y) { m_world_y = y; }

private:
	double m_world_x;
	double m_world_y;
};

class DrawObject : public GameObject
{
public:
	DrawObject(std::string img_name) : m_img_name(img_name) { }
	~DrawObject() { }

	const std::string &getImageName() { return m_img_name; }

protected:
	std::string m_img_name;

};