#include "monster.h"
#include "level.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	Monster::Monster(Texture2d* tex, Level* level)
		: level(level)
		, _in_idle(true)
	{
		sprite_g.create(tex);

		graphic = &sprite_g;

		size = { 14, 23 };
		origin = { 25, 41 };

		solid = true;

		sprite_g.frame_size = { 64, 64 };

		cur_dir = CUR_DIR_DOWN;

		SpriteAnim anim = { "idle_down",{ 0 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "idle_up",{ 8 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "idle_right",{ 16 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "idle_left",{ 24 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "down",{ 1, 2, 3, 4, 5, 6 }, 11.5f, true };
		sprite_g.add_anim(anim);

		anim = { "left",{ 25, 26, 27, 28, 29, 30 }, 11.5f, true };
		sprite_g.add_anim(anim);

		anim = { "right",{ 17, 18, 19, 20, 21, 22 }, 11.5f, true };
		sprite_g.add_anim(anim);

		anim = { "up",{ 9, 10, 11, 12, 13, 14 }, 11.5f, true };
		sprite_g.add_anim(anim);
	}

	Monster::~Monster()
	{
		_get_idle_move_stay_times();
	}

	void Monster::_get_idle_move_stay_times()
	{
		_idle_stuff._move_time = rand.get(_idle_stuff.MOVE_TIME_MIN, _idle_stuff.MOVE_TIME_MAX);
		_idle_stuff._stay_time = rand.get(_idle_stuff.STAY_TIME_MIN, _idle_stuff.STAY_TIME_MAX);
	}

	void Monster::update(float dt)
	{
		Entity::update(dt);

		if (_in_idle)
		{
			_do_idle(dt);
		}

		if (position.x + origin.x < 0)
			position.x = 0 - origin.x;
		if (position.y + origin.y < 0)
			position.y = 0 - origin.y;
		if (position.x + origin.x + size.x > level->size.x)
			position.x = level->size.x - (origin.x + size.x);
		if (position.y + origin.y + size.y > level->size.y)
			position.y = level->size.y - (origin.y + size.y);

		set_layer(position.y + origin.y + size.y);
	}

	void Monster::_do_idle(float dt)
	{
		std::string cur_anim = "idle_down";

		if (!_idle_stuff._moving)
		{
			_idle_stuff._stay_timer += dt;

			if (_idle_stuff._stay_timer >= _idle_stuff._stay_time)
			{
				_idle_stuff._moving = true;
				_idle_stuff._stay_timer = 0.0f;

				_get_idle_move_stay_times();

				float angle = rand.get(0, 2 * vcm::PI);

				_idle_stuff._move_dir = { cos(-angle), sin(-angle) };
			}
			else
			{

			}
		}

		if (_idle_stuff._moving)
		{
			_idle_stuff._move_timer += dt;
			
			if (_idle_stuff._move_timer >= _idle_stuff._move_time)
			{
				_idle_stuff._moving = false;
				_idle_stuff._move_timer = 0.0f;
			}
			else
			{
				cur_anim = "down";

				move(_idle_stuff._move_dir * SPEED * dt);
			}
		}

		sprite_g.play_anim(cur_anim);
	}
}