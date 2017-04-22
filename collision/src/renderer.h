#pragma once

#include "shader.h"

#include <vecmath/matrix.hpp>

namespace jaw
{
	struct SpriteGraphic;

	struct Renderer
	{
		Renderer();

		bool init();
		void clean();

		void render(SpriteGraphic* sprite, const vcm::mat4& transform);

		Shader sprite_shader;

		vcm::mat4 proj_mat;
	};
}