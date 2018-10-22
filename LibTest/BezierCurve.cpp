#include "BezierCurve.h"

BezierCurve::BezierCurve() : recursion_count(0)
{

}
BezierCurve::~BezierCurve()
{

}

void BezierCurve::addPoint(float x, float y)
{
	vec2d_f v;
	v.x = x;
	v.y = y;

	addPoint(v);
}
void BezierCurve::addPoint(vec2d_f v)
{
	control_points.push_back(v);
	if (control_points.size() > 1) calculate();
}

void BezierCurve::clearPoints()
{
	control_points.clear();
	curve_points.clear();
}

void BezierCurve::calculate()
{
	curve_points.clear();

	for (int i = 0; i <= SUBDIVS+1; ++i)
	{
		printf("Subdiv level: %.6f\n", float(i) / (SUBDIVS+1));
		curve_points.push_back(getMidpoints(control_points, float(i) / (SUBDIVS+1)).front());
		printf("Point calculated: %.2f, %.2f\n", curve_points.back().x, curve_points.back().y);
		recursion_count = 0;
	}

	printf("Calculated!\n");

}

std::vector<vec2d_f> BezierCurve::getMidpoints(std::vector<vec2d_f> vec, float dis)
{
	//if (recursion_count > 50) return std::vector<vec2d_f>();

	//recursion_count++;

	std::vector<vec2d_f> temp_points;

	for (int i = 0; i < vec.size() - 1; ++i)
	{
		vec2d_f point;

		point.x = vec[i].x + ((vec[i + 1].x - vec[i].x) * dis);
		point.y = vec[i].y + ((vec[i + 1].y - vec[i].y) * dis);

		temp_points.push_back(point);
	}

	if (temp_points.size() > 1) temp_points = getMidpoints(temp_points, dis);

	return temp_points;
	
}

void BezierCurve::draw(float xoff, float yoff)
{
	if (!control_points.empty())
	{
		for (int i = 0; i < control_points.size(); ++i)
		{
			//al_draw_line(control_points[i].x + xoff, control_points[i].y + yoff, control_points[i + 1].x + xoff, control_points[i + 1].y + yoff, al_map_rgb(255, 0, 255), 1);

			al_draw_filled_circle(control_points[i].x + xoff, control_points[i].y + yoff, 2, al_map_rgb(255, 0, 255));
		}
	}

	if (!curve_points.empty())
	{
		for (int i = 0; i < curve_points.size() - 1; ++i)
		{
			//al_draw_filled_circle(p.x + xoff, p.y + yoff, 2, al_map_rgb(255, 0, 0));

			al_draw_line(curve_points[i].x + xoff, curve_points[i].y + yoff, curve_points[i + 1].x + xoff, curve_points[i + 1].y + yoff, al_map_rgb(255, 0, 0), 1);
		}
	}
}
