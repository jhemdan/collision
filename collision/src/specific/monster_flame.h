#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"
#include "../graphic_group.h"
#include "../particle_graphic.h"

namespace jaw
{
	struct FlameSmoke;

	struct MonsterFlame : Entity
	{
		SpriteGraphic sprite_g;

		CurDir cur_dir;

		vcm::vec2 movement;

		float timer;
		bool alive;

		FlameSmoke* smoke;

		MonsterFlame(Texture2d* tex, Texture2d* particle_tex, const Point& pos, const vcm::vec2& movement);
		~MonsterFlame();

		void on_added() override;
		void update(float dt) override;
	};
}