#include "tilemap_graphic.h"
#include "exception.h"
#include "renderer.h"
#include "entity.h"
#include "world.h"

#include <assert.h>

namespace jaw
{
	TilemapGraphic::TilemapGraphic()
	{
		w = h = 0;
		tile_w = tile_h = 0;
		texture = nullptr;
		_dirty = false;
	}

	void TilemapGraphic::create(Texture2d* tex, int w, int h, int tile_w, int tile_h)
	{
		assert(tex);
	
		this->texture = tex;

		this->model.mbuffers.create(this->model.mesh);

		this->w = w;
		this->h = h;
		this->tile_w = tile_w;
		this->tile_h = tile_h;

		this->tiles.resize(w * h);

		for (auto& t : this->tiles)
			t = -1;
	}

	void TilemapGraphic::destroy()
	{
		model.mbuffers.destroy();

		*this = {};
	}

	void TilemapGraphic::set_tile(int x, int y, int value)
	{
		assert(x >= 0 && x < w);
		assert(y >= 0 && y < h);

		tiles[y * w + x] = value;

		_dirty = true;
	}

	int TilemapGraphic::get_tile(int x, int y) const
	{
		assert(x >= 0 && x < w);
		assert(y >= 0 && y < h);

		return tiles[y * w + x];
	}

	void TilemapGraphic::update(float dt)
	{
		Graphic::update(dt);
	}

	void TilemapGraphic::render(Renderer* renderer, Entity* entity)
	{
		Graphic::render(renderer, entity);

		if (_dirty)
		{
			_dirty = false;
			_build_mesh();
		}

		vcm::mat4 tran_mat = vcm::mat4
		{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ (float)entity->position.x, (float)entity->position.y, 0, 1 }
		};

		renderer->render(this, vcm::inverse(entity->world->cam_tran) * tran_mat);
	}

	void TilemapGraphic::_build_mesh()
	{
		auto& verts = model.mesh.vertices;
		auto& tris = model.mesh.triangles;

		verts.clear();
		tris.clear();

		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				int tile = get_tile(x, y);

				vcm::vec2 top_left = { (float)x * tile_w, (float)y * tile_h };
				vcm::vec2 bottom_right = top_left + vcm::vec2{ (float)tile_w, (float)tile_h };
				
				int tex_x = tile % w * tile_w;
				int tex_y = tile / w * tile_h;

				float ws = 1.0f / texture->w;
				float hs = 1.0f / texture->h;
				vcm::vec2 tex_top_left = { (float)tex_x * ws, (float)tex_y * hs };
				vcm::vec2 tex_bottom_right = tex_top_left + vcm::vec2{ tile_w * ws, tile_h * hs };

				unsigned vc = (unsigned)verts.size();

				verts.push_back({ {top_left.x, top_left.y}, {tex_top_left.x, tex_top_left.y} });
				verts.push_back({ {top_left.x, bottom_right.y}, {tex_top_left.x, tex_bottom_right.y} });
				verts.push_back({ {bottom_right.x, bottom_right.y}, {tex_bottom_right.x, tex_bottom_right.y} });
				verts.push_back({ {bottom_right.x, top_left.y}, {tex_bottom_right.x, tex_top_left.y} });

				tris.push_back(MeshTriangle{ 0, 1, 2 } + vc);
				tris.push_back(MeshTriangle{ 2, 3, 0 } + vc);
			}
		}

		model.mbuffers.recreate(model.mesh);
	}
}