#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"

namespace jaw
{
	struct MonsterFlame : Entity
	{
		SpriteGraphic sprite_g;

		CurDir cur_dir;

		vcm::vec2 movement;

		float timer;
		bool alive;

		MonsterFlame(Texture2d* tex, const Point& pos, const vcm::vec2& movement);

		void update(float dt) override;
	};
}