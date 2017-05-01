#include "tilemap_graphic.h"
#include "exception.h"

#include <assert.h>

namespace jaw
{
	TilemapGraphic::TilemapGraphic()
	{
		w = h = 0;
		tile_w = tile_h = 0;
		texture = nullptr;
	}

	void TilemapGraphic::create(Texture2d* tex, int w, int h, int tile_w, int tile_h)
	{
		assert(tex);
	
		this->texture = tex;

		this->model.mbuffers.create(this->model.mesh);
	}

	void TilemapGraphic::destroy()
	{

	}
}