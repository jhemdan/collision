#pragma once

#include "shader.h"

#include <vecmath/matrix.hpp>

namespace jaw
{
	struct SpriteGraphic;
	struct TilemapGraphic;
	struct TextGraphic;

	struct Renderer
	{
		Renderer();

		//throws exception on failure
		void init();
		void clean();

		void render(SpriteGraphic* sprite, const vcm::mat4& transform);
		void render(TilemapGraphic* tilemap, const vcm::mat4& transform);
		void render(TextGraphic* text, const vcm::mat4& transform);

		Shader sprite_shader;
		Shader text_shader;

		vcm::mat4 proj_mat;
	};
}