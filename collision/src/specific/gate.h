#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"

#include <vector>

namespace jaw
{
	struct Gate : Entity
	{
		SpriteGraphic sprite_g;

		std::vector<Entity*> boxes;

		Gate(Texture2d* tex);
		~Gate();

		void on_added() override;
		void on_removed() override;

		void open();
	};
}