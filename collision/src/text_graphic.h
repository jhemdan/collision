#pragma once

#include "graphic.h"
#include "model.h"

#include <string>
#include <vecmath/vector.hpp>

namespace jaw
{
	struct Font;

	struct TextGraphic : Graphic
	{
		TextGraphic();

		void create(Font* font);
		void destroy();

		void render(Renderer* renderer, Entity* entity) override;

		void set_text(const std::string& text);
		const std::string& get_text() const { return _text; }

		void set_word_wrap_width(int width);
		void set_word_wrap(bool val);

		void set_scale(float x, float y);
		const vcm::vec2& get_scale() const { return _scale; }

		void _build_mesh();

		Font* font;
		Model model;

		std::string _text;

		vcm::vec2 _scale;

		int _word_wrap_width;
		bool _word_wrap;

		vcm::vec4 color;
	};
}