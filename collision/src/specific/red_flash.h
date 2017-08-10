#pragma once

#include "../sprite_graphic.h"

namespace jaw
{
	struct RedFlash
	{
		bool flashing;
		float timer;
		SpriteGraphic* sprite;

		RedFlash();

		void hit();

		void update(float dt);
	};
}