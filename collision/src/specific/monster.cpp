#include "monster.h"
#include "level.h"
#include "../world.h"
#include "player.h"

#include "monster_flame.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	const float Monster::SPEED = 80.0f;

	const float Monster::MOVE_TIME_MIN = 0.2f;
	const float Monster::MOVE_TIME_MAX = 2.0f;
	const float Monster::STAY_TIME_MIN = 0.2f;
	const float Monster::STAY_TIME_MAX = 2.0f;

	const float Monster::CHASE_DIST = 150.0f;
	const float Monster::STOP_CHASE_DIST = 200.0f;
	const float Monster::ATTACK_DIST = 100.0f;

	const float Monster::CHASE_COOLDOWN = 0.2f;

	const float Monster::INITIAL_ATTACK_TIME = 1.5f;
	const float Monster::ATTACK_TIMER1_TIME = 2.5f;

	Monster::Monster(Texture2d* tex, Level* level)
		: level(level)
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

		anim = { "down",{ 1, 2, 3, 1, 2, 3 }, 11.5f, true };
		sprite_g.add_anim(anim);

		anim = { "left",{ 25, 26, 27, 25, 26, 27 }, 11.5f, true };
		sprite_g.add_anim(anim);

		anim = { "right",{ 17, 18, 19, 17, 18, 19 }, 11.5f, true };
		sprite_g.add_anim(anim);

		anim = { "up",{ 9, 10, 11, 9, 10, 11 }, 11.5f, true };
		sprite_g.add_anim(anim);

		_in_idle = true;
		_moving = false;
		_move_angle = 0.0f;
		_move_timer = 0.0f;
		_move_time = 0.0f;
		_stay_timer = 0.0f;
		_stay_time = 0.0f;
		_chase_cooldown = 0.0f;

		type = "monster";

		health = 3;

		_red_flash.sprite = &sprite_g;

		_attack_timer1 = 0.0f;

		_get_idle_move_stay_times();
	}

	Monster::~Monster()
	{
		
	}

	void Monster::_get_idle_move_stay_times()
	{
		_move_time = rand.get(MOVE_TIME_MIN, MOVE_TIME_MAX);
		_stay_time = rand.get(STAY_TIME_MIN, STAY_TIME_MAX);
	}

	void Monster::update(float dt)
	{
		Entity::update(dt);

		if (_in_idle)
		{
			_do_idle(dt);
		}
		else
		{
			_do_chase(dt);
		}

		float angle = _move_angle;
		cur_dir = cur_dir_from_angle(angle);

		std::string cur_anim = "idle_down";

		switch (cur_dir)
		{
		case CUR_DIR_RIGHT:
			if (_moving)
				cur_anim = "right";
			else
				cur_anim = "idle_right";
			break;
		case CUR_DIR_UP:
			if (_moving)
				cur_anim = "up";
			else
				cur_anim = "idle_up";
			break;
		case CUR_DIR_LEFT:
			if (_moving)
				cur_anim = "left";
			else
				cur_anim = "idle_left";
			break;
		case CUR_DIR_DOWN:
			if (_moving)
				cur_anim = "down";
			else
				cur_anim = "idle_down";
			break;
		}

		sprite_g.play_anim(cur_anim);

		if (position.x + origin.x < 0)
			position.x = 0 - origin.x;
		if (position.y + origin.y < 0)
			position.y = 0 - origin.y;
		if (position.x + origin.x + size.x > level->size.x)
			position.x = level->size.x - (origin.x + size.x);
		if (position.y + origin.y + size.y > level->size.y)
			position.y = level->size.y - (origin.y + size.y);

		set_layer(position.y + origin.y + size.y);

		_red_flash.update(dt);
	}

	void Monster::_do_idle(float dt)
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
				move(_move_dir * SPEED * dt);
			}
		}

		auto player = level->player;
		if (player && player->world)
		{
			auto diff = player->get_center_pos() - get_center_pos();
			vcm::vec2 diffv = (vcm::vec2)diff;
			float dist = vcm::length(diffv);

			if (dist <= CHASE_DIST)
			{
				_in_idle = false;
			}
		}
	}

	void Monster::_do_chase(float dt)
	{
		auto player = level->player;
		if (player && player->world)
		{
			auto diff = player->get_center_pos() - get_center_pos();
			vcm::vec2 diffv = (vcm::vec2)diff;
			float dist = vcm::length(diffv);

			if (dist < STOP_CHASE_DIST)
			{
				_move_dir = vcm::normalize(diffv);
				_move_angle = atan2(-_move_dir.y, _move_dir.x);
				if (_move_angle < 0)
					_move_angle += 2 * vcm::PI;

				if (dist > ATTACK_DIST)
				{
					if (_chase_cooldown == 0.0f)
					{
						_moving = true;

						move(_move_dir * SPEED * dt);
					}

					_chase_cooldown -= dt;

					if (_chase_cooldown < 0)
						_chase_cooldown = 0.0f;

					_attack_timer1 = INITIAL_ATTACK_TIME;
				}
				else
				{
					_moving = false;

					_chase_cooldown = CHASE_COOLDOWN;

					_attack_timer1 += dt;

					if (_attack_timer1 >= ATTACK_TIMER1_TIME)
					{
						_attack_timer1 = 0.0f;
						attack();
					}
				}
			}
			else
			{
				_in_idle = true;
			}
		}
		else
		{
			_in_idle = true;
		}
	}

	void Monster::take_hit()
	{
		health--;

		_red_flash.hit();

		if (health <= 0)
		{
			health = 0;

			die();
		}
	}

	void Monster::die()
	{
		world->remove_entity(this);
	}

	void Monster::attack()
	{
		auto player = level->player;
		if (player && player->world)
		{
			//player->take_hit();

			auto dir = player->get_center_pos() - get_center_pos();

			Point spawn_point;
			switch (cur_dir)
			{
			case CUR_DIR_DOWN:
				spawn_point = { 31, 42 };
				break;
			case CUR_DIR_UP:
				spawn_point = { 31, 42 };
				break;
			case CUR_DIR_RIGHT:
				spawn_point = { 43, 169 - 128 };
				break;
			case CUR_DIR_LEFT:
				spawn_point = { 19, 234 - (128 + 64) };
				break;
			}

			spawn_point -= Point(8, 8);

			auto flame = new MonsterFlame(&level->monster_flame_tex, position + spawn_point, vcm::normalize((vcm::vec2)dir) * 100);
			flame->destroy_on_remove = true;
			world->add_entity(flame);
		}
	}
}