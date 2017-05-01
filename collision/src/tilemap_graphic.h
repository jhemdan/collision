#pragma once

#include "texture_2d.h"
#include "model.h"

#include <vector>

namespace jaw
{
	struct TilemapGraphic
	{
		TilemapGraphic();

		void create(Texture2d* tex, int w, int h, int tile_w, int tile_h);
		void destroy();

		Texture2d* texture;
		Model model;
		int w, h;
		int tile_w, tile_h;
		std::vector<int> tiles;
	};
}