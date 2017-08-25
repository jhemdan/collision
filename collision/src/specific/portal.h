#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"

namespace jaw
{
	struct Portal : Entity
	{
		SpriteGraphic sprite_g;

		Portal(Texture2d* tex);
	};
}