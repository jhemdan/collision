#pragma once

#include <vecmath/vector.hpp>

namespace jaw
{
	struct Point
	{
		Point() : x(0), y(0) {}
		Point(int x, int y) : x(x), y(y) {}
		explicit Point(const vcm::vec2& v) : x((int)v.x), y((int)v.y) {}

		Point operator+(const Point& other) const
		{
			return Point(x + other.x, y + other.y);
		}

		Point& operator+=(const Point& other)
		{
			x += other.x;
			y += other.y;
			return *this;
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

		Point operator*(const vcm::vec2& vf) const
		{
			return Point((int)(x * vf.x), (int)(y * vf.y));
		}

		explicit operator vcm::vec2() const
		{
			return vcm::vec2{ (float)x, (float)y };
		}

		Point operator-() const
		{
			return Point(-x, -y);
		}

		int x, y;
	};
}