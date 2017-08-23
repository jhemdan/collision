#include "monster_flame.h"
#include "../world.h"
#include "player.h"
#include "flame_smoke.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	MonsterFlame::MonsterFlame(Texture2d* tex, Texture2d* particle_tex, const Point& pos, const vcm::vec2& movement)
	{
		position = pos;
		graphic = &sprite_g;

		sprite_g.create(tex);

		sprite_g.frame_size = { 16, 16 };

		SpriteAnim anim = { "right",{ 0, 1, 2, 3 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "down",{ 4, 5, 6, 7 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "left",{ 8, 9, 10, 11 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "up",{ 12, 13, 14, 15 }, 8, true };
		sprite_g.add_anim(anim);

		solid = false;

		this->movement = movement;

		auto move_dir = vcm::normalize(movement);
		float angle = atan2(-move_dir.y, move_dir.x);
		if (angle < 0)
			angle += 2 * vcm::PI;

		cur_dir = cur_dir_from_angle(angle);

		timer = 4.0f;

		alive = true;

		smoke = new FlameSmoke(particle_tex, move_dir);
	}

	void MonsterFlame::on_added()
	{
		Entity::on_added();

		world->add_entity(smoke);
	}

	void MonsterFlame::update(float dt)
	{
		Entity::update(dt);

		std::string cur_anim = "down";

		switch (cur_dir)
		{
		case CUR_DIR_RIGHT:
			cur_anim = "right";

			size = { 8, 8 };
			origin = { 7, 5 };
			break;
		case CUR_DIR_DOWN:
			cur_anim = "down";

			size = { 8, 8 };
			origin = { 3, 23 - 16 };
			break;
		case CUR_DIR_LEFT:
			cur_anim = "left";

			size = { 8, 8 };
			origin = { 2, 35 - 32 };
			break;
		case CUR_DIR_UP:
			cur_anim = "up";

			size = { 8, 8 };
			origin = { 5, 50 - 48 };
			break;
		}

		sprite_g.play_anim(cur_anim);

		move(movement * dt);

		set_layer(position.y + origin.y + size.y + 19);

		timer -= dt;
		if (timer <= 0.0f && alive)
		{
			world->remove_entity(this);
			alive = false;
		}

		std::vector<Entity*> isecs;
		if (alive && check_intersection(position, "player", isecs))
		{
			if (!isecs.empty())
			{
				auto player = (Player*)isecs[0];
				//player->take_hit();

				world->remove_entity(this);
				alive = false;
			}
		}

		smoke->alive = alive;
		smoke->emit_pos = position + Point(8, 8);

		smoke->set_layer(get_layer() - 1);
	}
}