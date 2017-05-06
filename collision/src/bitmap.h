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
		void destroy();

		//calcs the pitch from w and format
		void calc_pitch();

		int w, h;
		int pitch;
		BitmapFormat format;
		std::vector<ubyte> data;
	};
}