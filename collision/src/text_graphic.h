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

		void _build_mesh();

		Font* font;
		Model model;

		std::string _text;

		vcm::vec2 scale;
	};
}