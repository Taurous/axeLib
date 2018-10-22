#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

const int SUBDIVS = 19;

struct vec2d_f
{
	float x;
	float y;
};

class BezierCurve
{
public:
	BezierCurve();
	~BezierCurve();

	void addPoint(float x, float y);
	void addPoint(vec2d_f v);

	void clearPoints();

	void calculate();
	std::vector<vec2d_f> getMidpoints(std::vector<vec2d_f> vec, float dis);

	void draw(float xoff = 0.f, float yoff = 0.f);

private:
	int recursion_count;

	std::vector<vec2d_f> control_points;
	std::vector<vec2d_f> curve_points;
};