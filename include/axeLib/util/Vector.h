#pragma once

namespace axe
{
	class Vector2f
	{
	public:
		Vector2f() : x(0.f), y(0.f) { }
		~Vector2f() { }

	private:
		double x;
		double y;
	};
}