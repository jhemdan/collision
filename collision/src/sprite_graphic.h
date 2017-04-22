#pragma once

#include "graphic.h"
#include "texture_2d.h"
#include "model.h"

#include <vecmath/matrix.hpp>

namespace jaw
{
	struct SpriteGraphic : Graphic
	{
		SpriteGraphic();

		void create(Texture2d* tex);
		void destroy();

		void render(Renderer* renderer, Entity* entity) override;

		Model model;
		Texture2d* texture;
	};
}