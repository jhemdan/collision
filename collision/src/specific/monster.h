#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"
//#include "../random_source.h"
#include "red_flash.h"
#include "idle_walking.h"

namespace jaw
{
	struct Level;

	struct Monster : Entity
	{
		static const float SPEED;

		static const float CHASE_DIST;
		static const float STOP_CHASE_DIST;
		static const float ATTACK_DIST;

		static const float CHASE_COOLDOWN;

		static const float INITIAL_ATTACK_TIME;
		static const float ATTACK_TIMER1_TIME;

		SpriteGraphic sprite_g;

		CurDir cur_dir;

		bool _in_idle;
		IdleWalking _idle_walking;

		float _chase_cooldown;

		Level* level;

		int health;

		RedFlash _red_flash;

		float _attack_timer1;

		Monster(Texture2d* tex, Level* level);
		~Monster();

		void _do_idle(float dt);
		void _do_chase(float dt);

		void update(float dt) override;

		//the direction from the hitter to this monster
		void take_hit();
		void die();

		void attack();
	};
}