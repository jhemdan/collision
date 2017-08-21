#include "text_graphic.h"
#include "jaw_macros.h"
#include "renderer.h"
#include "entity.h"
#include "world.h"
#include "font.h"

#include <iostream>

namespace jaw
{
	TextGraphic::TextGraphic()
		: font(nullptr)
		, _scale(1.0f)

		, _word_wrap(false)
		, _word_wrap_width(0)

		, color(1.0f)
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

	void TextGraphic::render(Renderer* renderer, Entity* entity, const Point& offset)
	{
		Graphic::render(renderer, entity, offset);

		Point p = entity->position + position + offset;
		vcm::mat4 tran_mat = vcm::mat4
		{
			{ _scale.x, 0, 0, 0 },
			{ 0, _scale.y, 0, 0 },
			{ 0, 0, 1.0f, 0 },
			{ (float)p.x, (float)p.y, 0, 1 }
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

		//TODO: tabs?

		int prev = -1;
		for (auto cp = &_text[0]; *cp != 0; ++cp)
		{
			auto c = *cp;

			if (c == '\n' || c == '\r')
			{
				pen.y += font->line_height;
				pen.x = 0;

				prev = (int)c;
				continue;
			}

			Font::Glyph glyph = font->get_glyph((int)c);
			//Font::Kerning kerning = font->get_kerning(prev, (int)c); //TODO

			if (glyph.id == -1)
			{
				pen.x += 1;

				prev = (int)c;
				continue;
			}

			if (_word_wrap)
			{
				if ((prev == ' ' || prev == '\n' || prev == '\r' || ispunct(prev)) && (isalpha(c) || isdigit(c)))
				{
					int min = (int)(pen.x * _scale.x);
					if (min >= _word_wrap_width)
					{
						pen.x = 0;
						pen.y += font->line_height;
					}
					else
					{
						int max = pen.x;

						char* last_letter = cp;
						for (auto cp2 = cp; *cp2 != 0; ++cp2)
						{
							if (*cp2 == ' ' || *cp2 == '\n' || *cp2 == '\r' || ispunct(*cp2))
								break;

							last_letter = cp2;
						}

						for (auto i = cp; i != last_letter + 1; ++i)
						{
							Font::Glyph glyph2 = font->get_glyph((int)*i);
							max += glyph2.xoffset;
							max += glyph2.width;
							max += glyph2.xadvance;
						}
						
						max = (int)(max * _scale.x);

						if (max >= _word_wrap_width)
						{
							pen.x = 0;
							pen.y += font->line_height;
						}
					}
				}
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

	void TextGraphic::set_word_wrap_width(int width)
	{
		if (_word_wrap_width != width)
		{
			_word_wrap_width = width;
			_build_mesh();
		}
	}

	void TextGraphic::set_word_wrap(bool val)
	{
		if (_word_wrap != val)
		{
			_word_wrap = val;
			_build_mesh();
		}
	}

	void TextGraphic::set_scale(float x, float y)
	{
		if (_scale != vcm::vec2(x, y))
		{
			_scale = { x, y };
			_build_mesh();
		}
	}
}