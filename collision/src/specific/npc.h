#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"
#include "idle_walking.h"

namespace jaw
{
	struct Level;

	struct NPC : Entity
	{
		static const float SPEED;

		SpriteGraphic sprite_g;

		CurDir cur_dir;

		bool _in_idle;
		IdleWalking _idle_walking;
	
		Level* level;

		NPC(Texture2d* tex, Level* level);

		void update(float dt) override;
		
		void _do_idle(float dt);
	};
}