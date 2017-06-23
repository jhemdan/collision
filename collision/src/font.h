#pragma once

#include "bitmap.h"
#include "texture_2d.h"

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
		Texture2d texture;
		std::map<int, Glyph> glyphs;
		std::map<std::pair<int, int>, Kerning> kernings;
		
		//returns the glyph with this id
		//returns a glyph with a -1 id if no glyph was find
		Glyph get_glyph(int id) const
		{
			auto it = glyphs.find(id);
			if (it != glyphs.end())
				return it->second;

			Glyph g;
			g.id = -1;
			return g;
		}

		//returns the kerning with this first and second
		//returns a kerning with -1 for first and second if no kerning was found
		Kerning get_kerning(int first, int second) const
		{
			auto it = kernings.find(std::make_pair(first, second));
			if (it != kernings.end())
				return it->second;

			Kerning k;
			k.first = k.second = -1;
			return k;
		}

		Font();

		void create(const std::string& path);
		void destroy();

		int line_height;
	};
}