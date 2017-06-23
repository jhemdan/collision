#include "text_graphic.h"
#include "jaw_macros.h"
#include "renderer.h"
#include "entity.h"
#include "world.h"
#include "font.h"

namespace jaw
{
	TextGraphic::TextGraphic()
		: font(nullptr)
		, scale(1.0f)
	{

	}

	void TextGraphic::create(Font* font)
	{
		JAW_ASSERT_MSG(font, "No font supplied to TextGraphic::create()");

		this->font = font;

		this->model.mbuffers.create(this->model.mesh);
	}

	void TextGraphic::destroy()
	{
		model.mbuffers.destroy();

		*this = {};
	}

	void TextGraphic::render(Renderer* renderer, Entity* entity)
	{
		Graphic::render(renderer, entity);

		vcm::mat4 tran_mat = vcm::mat4
		{
			{ scale, 0, 0, 0 },
			{ 0, scale, 0, 0 },
			{ 0, 0, scale, 0 },
			{ (float)entity->position.x, (float)entity->position.y, 0, 1 }
		};

		renderer->render(this, vcm::inverse(entity->world->cam_tran) * tran_mat);
	}

	void TextGraphic::set_text(const std::string& t)
	{
		if (_text != t)
		{
			_text = t;
			_build_mesh();
		}
	}

	void TextGraphic::_build_mesh()
	{
		auto& verts = model.mesh.vertices;
		auto& tris = model.mesh.triangles;

		verts.clear();
		tris.clear();

		Point pen;

		int prev = -1;
		for (auto c : _text)
		{
			if (c == '\n' || c == '\r')
			{
				pen.y += font->line_height;
				pen.x = 0;
				continue;
			}

			Font::Glyph glyph = font->get_glyph((int)c);
			Font::Kerning kerning = font->get_kerning(prev, (int)c);

			if (glyph.id == -1)
			{
				pen.x += 1;
				continue;
			}

			int x = pen.x + glyph.xoffset;
			int y = pen.y + glyph.yoffset;

			vcm::vec2 top_left = { (float)x, (float)y };
			vcm::vec2 bottom_right = top_left + vcm::vec2{ (float)glyph.width, (float)glyph.height };

			int tex_x = glyph.x;
			int tex_y = glyph.y;

			float ws = 1.0f / font->texture.w;
			float hs = 1.0f / font->texture.h;
			vcm::vec2 tex_top_left = { (float)tex_x * ws, (float)tex_y * hs };
			vcm::vec2 tex_bottom_right = tex_top_left + vcm::vec2{ glyph.width * ws, glyph.height * hs };

			unsigned vc = (unsigned)verts.size();

			verts.push_back({ { top_left.x, top_left.y },{ tex_top_left.x, tex_top_left.y } });
			verts.push_back({ { top_left.x, bottom_right.y },{ tex_top_left.x, tex_bottom_right.y } });
			verts.push_back({ { bottom_right.x, bottom_right.y },{ tex_bottom_right.x, tex_bottom_right.y } });
			verts.push_back({ { bottom_right.x, top_left.y },{ tex_bottom_right.x, tex_top_left.y } });

			tris.push_back(MeshTriangle{ 0, 1, 2 } +vc);
			tris.push_back(MeshTriangle{ 2, 3, 0 } +vc);

			pen.x += glyph.xadvance;

			prev = (int)c;
		}

		model.mbuffers.recreate(model.mesh);
	}
}