#pragma once

#include "bitmap.h"

#include <map>
#include <string>
#include <utility>

namespace jaw
{
	struct Font
	{
		struct Glyph
		{
			int id;
			int x, y, width, height;
			int xoffset, yoffset;
			int xadvance;
		};

		struct Kerning
		{
			int first, second;
			int amount;
		};

		Bitmap image;
		std::map<int, Glyph> glyphs;
		std::map<std::pair<int, int>, Kerning> kernings;

		Font();

		void create(const std::string& path);
		void destroy();
	};
}