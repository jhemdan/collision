#pragma once

#include "../random_source.h"

#include <vecmath/vector.hpp>

namespace jaw
{
	struct IdleWalking
	{
		static const float MOVE_TIME_MIN;
		static const float MOVE_TIME_MAX;
		static const float STAY_TIME_MIN;
		static const float STAY_TIME_MAX;

		bool _moving;
		vcm::vec2 _move_dir;
		float _move_angle;
		float _move_timer;
		float _move_time;
		float _stay_timer;
		float _stay_time;
	
		RandomSource rand;

		IdleWalking();

		void update(float dt);

		void _get_idle_move_stay_times();

		bool is_moving() const { return _moving; }
		const vcm::vec2& get_move_dir() const { return _move_dir; }
		float get_move_angle() const { return _move_angle; }
	};
}