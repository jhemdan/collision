#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "../texture_2d.h"

namespace jaw
{
	struct Bush : Entity
	{
		SpriteGraphic sprite_g;

		Bush(Texture2d* tex);
		~Bush();

		void on_added() override;
		void on_removed() override;
	};
}