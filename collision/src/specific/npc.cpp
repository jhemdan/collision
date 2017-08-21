#include "npc.h"

#include "level.h"
#include "player.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	const float NPC::SPEED = 80.0f;
	const float NPC::STOP_TALKING_DIST = 100.0f;

	NPC::NPC(Texture2d* tex, Level* level)
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

		_in_idle = true;

		this->level = level;

		type = "npc";

		_set_up_dialogue();
	}

	NPC::~NPC()
	{
		
	}

	void NPC::update(float dt)
	{
		Entity::update(dt);

		if (_in_idle)
		{
			_do_idle(dt);
		}
		else
		{
			_do_talking(dt);
		}

		float angle = _idle_walking.get_move_angle();
		cur_dir = cur_dir_from_angle(angle);

		std::string cur_anim = "idle_down";

		switch (cur_dir)
		{
		case CUR_DIR_RIGHT:
			if (_idle_walking.is_moving())
				cur_anim = "right";
			else
				cur_anim = "idle_right";
			break;
		case CUR_DIR_UP:
			if (_idle_walking.is_moving())
				cur_anim = "up";
			else
				cur_anim = "idle_up";
			break;
		case CUR_DIR_LEFT:
			if (_idle_walking.is_moving())
				cur_anim = "left";
			else
				cur_anim = "idle_left";
			break;
		case CUR_DIR_DOWN:
			if (_idle_walking.is_moving())
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
	}

	void NPC::_do_idle(float dt)
	{
		_idle_walking.update(dt);

		if (_idle_walking.is_moving())
		{
			move(_idle_walking.get_move_dir() * SPEED * dt);
		}

		level->player_hud.hide_text_box();
	}

	void NPC::_do_talking(float dt)
	{
		auto player = level->player;
		if (player)
		{
			auto diff = player->get_center_pos() - get_center_pos();
			vcm::vec2 diffv = (vcm::vec2)diff;
			float dist = vcm::length(diffv);

			if (dist >= STOP_TALKING_DIST)
			{
				_in_idle = true;
			}

			_idle_walking._move_dir = vcm::normalize(diffv);
			_idle_walking._move_angle = atan2(-_idle_walking._move_dir.y, _idle_walking._move_dir.x);
			if (_idle_walking._move_angle < 0)
				_idle_walking._move_angle += 2 * vcm::PI;

			_idle_walking._moving = false;
		}
		else
		{
			_in_idle = true;
		}
	}

	void NPC::talk()
	{
		if (_in_idle)
		{
			_in_idle = false;

			level->player_hud.show_text_box(&_dialogue);
		}
	}

	void NPC::stop_talking()
	{
		if (!_in_idle)
		{
			_in_idle = true;
		}
	}

	void NPC::_set_up_dialogue()
	{
		_dialogue.add("Jawdat: What's up Ibrahim? I need some help. Can you please help me?", 0);
		_dialogue.add("Jawdat: Awesome. I need three monster eyes for a potion. Here's the key to that gate up there.", 1);
		_dialogue.add("Jawdat: Press 'Z' to open the gate. Press 'X' to attack. Good luck.", 0);
	}
}