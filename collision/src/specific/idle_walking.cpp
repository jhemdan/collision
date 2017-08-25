#include "idle_walking.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	const float IdleWalking::MOVE_TIME_MIN = 0.2f;
	const float IdleWalking::MOVE_TIME_MAX = 1.5f;
	const float IdleWalking::STAY_TIME_MIN = 0.2f;
	const float IdleWalking::STAY_TIME_MAX = 2.0f;

	IdleWalking::IdleWalking()
	{
		_moving = false;
		_move_angle = 0.0f;
		_move_timer = 0.0f;
		_move_time = 0.0f;
		_stay_timer = 0.0f;
		_stay_time = 0.0f;

		_get_idle_move_stay_times();
	}

	void IdleWalking::update(float dt)
	{
		if (!_moving)
		{
			_stay_timer += dt;

			if (_stay_timer >= _stay_time)
			{
				_moving = true;
				_stay_timer = 0.0f;

				_get_idle_move_stay_times();

				float angle = rand.get(0, 2 * vcm::PI);

				_move_dir = { cos(-angle), sin(-angle) };
				_move_angle = angle;
			}
			else
			{

			}
		}

		if (_moving)
		{
			_move_timer += dt;

			if (_move_timer >= _move_time)
			{
				_moving = false;
				_move_timer = 0.0f;
			}
			else
			{
				
			}
		}
	}

	void IdleWalking::_get_idle_move_stay_times()
	{
		_move_time = rand.get(MOVE_TIME_MIN, MOVE_TIME_MAX);
		_stay_time = rand.get(STAY_TIME_MIN, STAY_TIME_MAX);
	}
}