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

		bool create(const std::string& file_name);
		void destroy();

		int w, h;
		int pitch;
		BitmapFormat format;
		std::vector<ubyte> data;
	};
}