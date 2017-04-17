#pragma once

#include "texture_2d.h"
#include "model.h"

#include <vecmath/vector.hpp>

namespace jaw
{
	struct Sprite
	{
		Sprite();

		void create(Texture2d* tex);
		void destroy();

		Model model;
		Texture2d* texture;
		vcm::vec2 position;
	};
}