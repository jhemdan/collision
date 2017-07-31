#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"
#include "../random_source.h"

namespace jaw
{
	struct Level;

	struct Monster : Entity
	{
		SpriteGraphic sprite_g;

		CurDir cur_dir;

		const float SPEED = 100.0f;

		struct IdleStuff
		{
			const float MOVE_TIME_MIN = 0.2f;
			const float MOVE_TIME_MAX = 2.0f;
			const float STAY_TIME_MIN = 0.2f;
			const float STAY_TIME_MAX = 2.0f;

			IdleStuff()
				: _moving(false)
				, _move_timer(0.0f)
				, _move_time(0.0f)
				, _move_angle(0.0f)

				, _stay_timer(0.0f)
				, _stay_time(0.0f)
			{

			}

			bool _moving;
			vcm::vec2 _move_dir;
			float _move_angle;
			float _move_timer;
			float _move_time;
			float _stay_timer;
			float _stay_time;
		};

		bool _in_idle;
		IdleStuff _idle_stuff;

		RandomSource rand;

		Level* level;

		int health;

		bool _flashing_red;
		float _red_timer;

		Monster(Texture2d* tex, Level* level);
		~Monster();

		void _do_idle(float dt);
		void _get_idle_move_stay_times();

		void update(float dt) override;

		void take_hit();
		void die();
	};
}