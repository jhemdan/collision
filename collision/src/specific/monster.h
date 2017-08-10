#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"
#include "../random_source.h"
#include "red_flash.h"

namespace jaw
{
	struct Level;

	struct Monster : Entity
	{
		static const float SPEED;

		static const float MOVE_TIME_MIN;
		static const float MOVE_TIME_MAX;
		static const float STAY_TIME_MIN;
		static const float STAY_TIME_MAX;

		static const float CHASE_DIST;
		static const float STOP_CHASE_DIST;
		static const float ATTACK_DIST;

		static const float CHASE_COOLDOWN;

		static const float INITIAL_ATTACK_TIME;
		static const float ATTACK_TIMER1_TIME;

		SpriteGraphic sprite_g;

		CurDir cur_dir;

		bool _in_idle;
		bool _moving;
		vcm::vec2 _move_dir;
		float _move_angle;
		float _move_timer;
		float _move_time;
		float _stay_timer;
		float _stay_time;
		float _chase_cooldown;

		RandomSource rand;

		Level* level;

		int health;

		RedFlash _red_flash;

		float _attack_timer1;

		Monster(Texture2d* tex, Level* level);
		~Monster();

		void _do_idle(float dt);
		void _get_idle_move_stay_times();
		void _do_chase(float dt);

		void update(float dt) override;

		void take_hit();
		void die();

		void attack();
	};
}