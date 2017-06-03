#pragma once

#include "types.h"

#include <vector>

namespace jaw
{
	enum BitmapFormat
	{
		BITMAP_UNDEFINED,
		BITMAP_GRAY,
		BITMAP_RGB,
		BITMAP_RGBA
	};

	struct Bitmap
	{
		Bitmap();

		//throws exception on failure
		void create(const std::string& file_name);
		void create(int w, int h, BitmapFormat format);
		void destroy();

		//calcs the pitch from w and format
		void calc_pitch();

		void set_pixel(int x, int y, unsigned value);
		void draw_line(int x1, int y1, int x2, int y2, unsigned value);

		int w, h;
		int pitch;
		BitmapFormat format;
		std::vector<ubyte> data;
	};
}