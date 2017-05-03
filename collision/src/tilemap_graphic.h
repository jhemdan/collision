#pragma once

#include "texture_2d.h"
#include "model.h"
#include "graphic.h"

#include <vector>

namespace jaw
{
	struct TilemapGraphic : Graphic
	{
		TilemapGraphic();

		void create(Texture2d* tex, int w, int h, int tile_w, int tile_h);
		void destroy();

		void set_tile(int x, int y, int value);
		int get_tile(int x, int y) const;

		void update(float dt) override;
		void render(Renderer* renderer, Entity* entity) override;

		void _build_mesh();

		Texture2d* texture;
		Model model;
		int w, h;
		int tile_w, tile_h;
		std::vector<int> tiles;
		
		bool _dirty;
	};
}