#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

const int SUBDIVS = 99;

struct vec2d_f
{
	float x;
	float y;
};

struct arrow
{
	float x1, y1, x2, y2, x3, y3, x4, y4;
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
	std::vector<arrow> arrows;
	std::vector<vec2d_f> curve_points;
};