#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "../texture_2d.h"

#include <vector>

namespace jaw
{
	struct OakTree : Entity
	{
		SpriteGraphic tree_g;

		std::vector<Entity*> boxes;
	
		OakTree(Texture2d* tex);
		~OakTree();

		void on_added() override;
		void on_removed() override;
	};
}