#include "player.h"

#include "../world.h"
#include "../game.h"
#include "level.h"
#include "../cam_ent.h"
#include "monster.h"

namespace jaw
{
	const float Player::SPEED = 100.0f;
	const float Player::CAM_SPEED = 8.0f;

	const float Player::SWORD_COLLIDER_SIZE_X_LR = 0.8f;
	const float Player::SWORD_COLLIDER_SIZE_X_UD = 0.2f;

	const float Player::ATTACK_COOLDOWN = .69f;

	Player::Player(Texture2d* tex, Level* level)
		: sword_tex(nullptr)
		, magenta_tex(nullptr)

		, attack_timer(0.0f)
		, slash_moment(1 / 14.0f)
		, has_attacked(false)

		, _cool_down(0.0f)
	{
		sprite_g.create(tex);

		graphic = &sprite_g;

		size = { 14, 23 };
		origin = { 25, 41 };

		position = { 0, 0 };

		sprite_g.set_clip_rect(Rect(0, 0, 64, 64));

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

		this->level = level;

		Bitmap sword_bmp;
		sword_bmp.create("../assets/sword.png");
		sword_tex = new Texture2d();
		sword_tex->create(sword_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		sword_g.create(sword_tex);

		sword_g.frame_size = { 64, 64 };

		anim = { "slash_r", { 0, 1, 2, 3 }, 14, false };
		sword_g.add_anim(anim);

		anim = { "slash_l",{ 4, 5, 6, 7 }, 14, false };
		sword_g.add_anim(anim);

		anim = { "slash_d",{ 8, 9, 10, 11 }, 14, false };
		sword_g.add_anim(anim);

		anim = { "slash_u",{ 12, 13, 14, 15 }, 14, false };
		sword_g.add_anim(anim);

		sword_ent.graphic = &sword_g;

		sword_g.visible = false;

		swinging = false;

		Bitmap magenta_bmp;
		magenta_bmp.create("../assets/magenta_box.png");
		magenta_tex = new Texture2d();
		magenta_tex->create(magenta_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		magenta_g.create(magenta_tex);
		magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_LR;
		magenta_g.scale.y = 0.75f;

		magenta_ent.graphic = &magenta_g;

		sword_collider.parent = &magenta_ent;

		magenta_g.visible = false;
	
		health = 3;
		//health = 100;

		type = "player";
	}

	Player::~Player()
	{
		sprite_g.destroy();
		
		delete sword_tex;
		delete magenta_tex;
	}

	Point Player::get_cam_dest_pos() const
	{
		return position + Point(32, 32) - Point(world->cam_ent->size * 0.5f);
	}

	void Player::on_added()
	{
		Entity::on_added();

		cam_posv = (vcm::vec2)get_cam_dest_pos();

		world->add_entity(&sword_ent);
		world->add_entity(&magenta_ent);
		world->add_entity(&sword_collider);
	}

	void Player::on_removed()
	{
		Entity::on_removed();

		world->remove_entity(&sword_ent);
		world->remove_entity(&magenta_ent);
		world->remove_entity(&sword_collider);
	}

	void Player::update(float dt)
	{
		Entity::update(dt);

		std::string cur_anim;

		switch (cur_dir)
		{
		case CUR_DIR_DOWN:
			cur_anim = "idle_down";
			break;
		case CUR_DIR_UP:
			cur_anim = "idle_up";
			break;
		case CUR_DIR_LEFT:
			cur_anim = "idle_left";
			break;
		case CUR_DIR_RIGHT:
			cur_anim = "idle_right";
			break;
		}

		dir = Point(0, 0);
		if (game.input.key_is_down(SDL_SCANCODE_RIGHT))
		{
			dir.x = 1;
			cur_anim = "right";
			cur_dir = CUR_DIR_RIGHT;
		}
		else if (game.input.key_is_down(SDL_SCANCODE_LEFT))
		{
			dir.x = -1;
			cur_anim = "left";
			cur_dir = CUR_DIR_LEFT;
		}
		if (game.input.key_is_down(SDL_SCANCODE_DOWN))
		{
			dir.y = 1;
			cur_anim = "down";
			cur_dir = CUR_DIR_DOWN;
		}
		else if (game.input.key_is_down(SDL_SCANCODE_UP))
		{
			dir.y = -1;
			cur_anim = "up";
			cur_dir = CUR_DIR_UP;
		}

		sprite_g.play_anim(cur_anim);

		move(vcm::normalize((vcm::vec2)dir) * SPEED * dt);

		set_layer(position.y + origin.y + size.y);

		auto cam_dest_pos = get_cam_dest_pos();

		cam_posv += (vcm::vec2)(cam_dest_pos - Point(cam_posv)) * CAM_SPEED * dt;

		if (cam_posv.x < 0)
			cam_posv.x = 0;
		if (cam_posv.y < 0)
			cam_posv.y = 0;
		if (cam_posv.x + world->cam_ent->size.x > level->size.x)
			cam_posv.x = level->size.x - world->cam_ent->size.x;
		if (cam_posv.y + world->cam_ent->size.y > level->size.y)
			cam_posv.y = level->size.y - world->cam_ent->size.y;

		world->cam_ent->position = Point(cam_posv);

		auto switch_sword_anim_mid_anim = [this](const std::string& anim_name)
		{
			if (swinging && sword_g.is_playing_anim())
			{
				const auto& anim = sword_g.get_anim(sword_g.get_cur_anim_index());

				if (anim.name != anim_name)
				{
					int cur_anim_frame = sword_g.get_cur_anim_frame();
					float anim_timer = sword_g.get_anim_timer();
					sword_g.play_anim(anim_name);
					sword_g.set_anim_frame(cur_anim_frame);
					sword_g.set_anim_timer(anim_timer);
				}
			}
		};

		if (cur_dir == CUR_DIR_RIGHT)
		{
			magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_LR;
			sword_collider.size = Point(magenta_tex->w, magenta_tex->h) * magenta_g.scale;

			sword_ent.position = position + Point(10, 0);
			magenta_ent.position = position + Point(43, 32);

			switch_sword_anim_mid_anim("slash_r");
		}
		else if (cur_dir == CUR_DIR_LEFT)
		{
			magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_LR;
			sword_collider.size = Point(magenta_tex->w, magenta_tex->h) * magenta_g.scale;

			sword_ent.position = position + Point(-10, 0);
			magenta_ent.position = position + Point(32 - sword_collider.size.x - 13, 32);

			switch_sword_anim_mid_anim("slash_l");
		}
		else if (cur_dir == CUR_DIR_DOWN)
		{
			magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_UD;
			sword_collider.size = Point(magenta_tex->w, magenta_tex->h) * magenta_g.scale;

			sword_ent.position = position + Point(0, 26);
			magenta_ent.position = position + Point(32 - (int)(sword_collider.size.x * 0.5f), 60);

			switch_sword_anim_mid_anim("slash_d");
		}
		else if (cur_dir == CUR_DIR_UP)
		{
			magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_UD;
			sword_collider.size = Point(magenta_tex->w, magenta_tex->h) * magenta_g.scale;

			sword_ent.position = position + Point(0, -5);
			magenta_ent.position = position + Point(32 - (int)(sword_collider.size.x * 0.5f), -3);

			switch_sword_anim_mid_anim("slash_u");
		}

		sword_ent.set_layer(sword_ent.position.y + 64 + 1);
		magenta_ent.set_layer(magenta_ent.position.y + (int)(magenta_g.texture->h * magenta_g.scale.y));

		_cool_down -= dt;
		if (_cool_down < 0.0f)
			_cool_down = 0.0f;

		if (!swinging && game.input.key_is_down(SDL_SCANCODE_X) && _cool_down == 0.0f)
		{
			std::string sword_anim;
			if (cur_dir == CUR_DIR_RIGHT)
			{
				sword_anim = "slash_r";
			}
			else if (cur_dir == CUR_DIR_LEFT)
			{
				sword_anim = "slash_l";
			}
			else if (cur_dir == CUR_DIR_DOWN)
			{
				sword_anim = "slash_d";
			}
			else if (cur_dir == CUR_DIR_UP)
			{
				sword_anim = "slash_u"; 
			}

			swinging = true;
			sword_g.play_anim(sword_anim);
			sword_g.visible = true;

			_cool_down = ATTACK_COOLDOWN;
		}

		if (swinging)
		{
			if (sword_g.has_reached_end() || !sword_g.is_playing_anim())
			{
				swinging = false;
				sword_g.visible = false;
				sword_g.stop_anim();

				attack_timer = 0.0f;
				has_attacked = false;
			}
			else
			{
				attack_timer += dt;
			}
		}

		if (attack_timer >= slash_moment && !has_attacked)
		{
			has_attacked = true;
			attack();
		}
	}

	void Player::attack()
	{
		std::vector<Entity*> es;
		if (sword_collider.check_intersection(sword_collider.position, "tree", es))
		{
			auto tree = es[0]->parent;
			world->remove_entity(tree);
		}

		es.clear();
		if (sword_collider.check_intersection(sword_collider.position, "monster", es))
		{
			for (auto e : es)
			{
				auto m = (Monster*)e;
				m->take_hit();
			}
		}
	}

	void Player::take_hit()
	{
		health--;

		if (health <= 0)
		{
			health = 0;

			die();
		}

		level->player_hud.set_hearts(health);
	}

	void Player::die()
	{
		world->remove_entity(this);
	}
}