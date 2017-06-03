#pragma once

#include <vecmath/vector.hpp>

namespace jaw
{
	struct Point
	{
		Point() : x(0), y(0) {}
		Point(int x, int y) : x(x), y(y) {}
		Point(const vcm::vec2& v) : x((int)v.x), y((int)v.y) {}

		Point operator+(const Point& other) const
		{
			return Point(x + other.x, y + other.y);
		}

		Point operator-(const Point& other) const
		{
			return Point(x - other.x, y - other.y);
		}

		Point operator*(const Point& other) const
		{
			return Point(x * other.x, y * other.y);
		}

		Point operator/(const Point& other) const
		{
			return Point(x / other.x, y / other.y);
		}

		Point operator*(int i) const
		{
			return Point(x * i, y * i);
		}

		Point operator*(float f) const
		{
			return Point((int)(x * f), (int)(y * f));
		}

		explicit operator vcm::vec2() const
		{
			return vcm::vec2{ (float)x, (float)y };
		}

		int x, y;
	};
}