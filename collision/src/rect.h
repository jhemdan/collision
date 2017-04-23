#pragma once

namespace jaw
{
	struct Rect
	{
		Rect() : x(0), y(0), w(0), h(0) {}
		Rect(int w, int h) : x(0), y(0), w(w), h(h) {}
		Rect(int x, int y, int w, int h)
			: x(x), y(y), w(w), h(h) {}

		bool operator==(const Rect& other) const
		{
			return x == other.x && y == other.y && w == other.w && h == other.h;
		}

		bool operator!=(const Rect& other) const
		{
			return !(*this == other);
		}

		int x, y, w, h;
	};
}