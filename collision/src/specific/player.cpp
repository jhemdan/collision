#include "player.h"

#include "../world.h"
#include "../game.h"
#include "level.h"
#include "../cam_ent.h"
#include "monster.h"
#include "npc.h"
#include "gate.h"
#include "blood_squirt.h"
#include "portal.h"

namespace jaw
{
	const float Player::SPEED = 100.0f;
	const float Player::CAM_SPEED = 8.0f;

	const float Player::SWORD_COLLIDER_SIZE_X_LR = 0.8f;
	const float Player::SWORD_COLLIDER_SIZE_X_UD = 0.2f;

	const float Player::ATTACK_COOLDOWN = .69f;

	Player::Player(Texture2d* tex, Level* level)
		: attack_timer(0.0f)
		, slash_moment(1 / 14.0f)
		, has_attacked(false)

		, _cool_down(0.0f)

		, npc(nullptr)
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
		sword_tex.create(sword_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		sword_g.create(&sword_tex);

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
		magenta_tex.create(magenta_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		magenta_g.create(&magenta_tex);
		magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_LR;
		magenta_g.scale.y = 0.75f;

		magenta_ent.graphic = &magenta_g;

		sword_collider.parent = &magenta_ent;

		magenta_g.visible = false;
	
		health = 3;
		//health = 100;
		_health_timer = 0.0f;

		type = "player";

		_red_flash.sprite = &sprite_g;
	}

	Player::~Player()
	{
		sprite_g.destroy();
		sword_g.destroy();
		magenta_g.destroy();

		sword_tex.destroy();
		magenta_tex.destroy();
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
			sword_collider.size = Point(magenta_tex.w, magenta_tex.h) * magenta_g.scale;

			sword_ent.position = position + Point(10, 0);
			magenta_ent.position = position + Point(43, 32);

			switch_sword_anim_mid_anim("slash_r");
		}
		else if (cur_dir == CUR_DIR_LEFT)
		{
			magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_LR;
			sword_collider.size = Point(magenta_tex.w, magenta_tex.h) * magenta_g.scale;

			sword_ent.position = position + Point(-10, 0);
			magenta_ent.position = position + Point(32 - sword_collider.size.x - 13, 32);

			switch_sword_anim_mid_anim("slash_l");
		}
		else if (cur_dir == CUR_DIR_DOWN)
		{
			magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_UD;
			sword_collider.size = Point(magenta_tex.w, magenta_tex.h) * magenta_g.scale;

			sword_ent.position = position + Point(0, 26);
			magenta_ent.position = position + Point(32 - (int)(sword_collider.size.x * 0.5f), 60);

			switch_sword_anim_mid_anim("slash_d");
		}
		else if (cur_dir == CUR_DIR_UP)
		{
			magenta_g.scale.x = SWORD_COLLIDER_SIZE_X_UD;
			sword_collider.size = Point(magenta_tex.w, magenta_tex.h) * magenta_g.scale;

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

		_red_flash.update(dt);

		/*
		if (game.input.key_pressed(SDL_SCANCODE_SPACE))
		{
			static Texture2d* other_tex = nullptr;
			if (!other_tex)
			{
				other_tex = new Texture2d();
				Bitmap bitmap;
				bitmap.create("../assets/jawdat.png");
				other_tex->create(bitmap, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
			}
			
			if (sprite_g.texture == other_tex)
				sprite_g.texture = &level->player_tex;
			else
				sprite_g.texture = other_tex;
		}
		*/

		level->player_hud.hide_to_talk_text();

		npc = nullptr;
		_ent_buff.clear();
		if (sword_collider.check_intersection(sword_collider.position, "npc", _ent_buff))
		{
			auto npc = (NPC*)_ent_buff[0];
			this->npc = npc;

			if(!npc->is_talking())
				level->player_hud.show_to_talk_text();
		}

		if (game.input.key_pressed(SDL_SCANCODE_Z))
		{
			if (npc && !npc->is_talking())
			{
				npc->talk();
			}
		}

		if (npc && !level->player_hud.is_showing_text_box())
		{
			npc->stop_talking();
		}

		level->player_hud.hide_portal_text();

		_ent_buff.clear();
		if (sword_collider.check_intersection(sword_collider.position, "portal", _ent_buff))
		{
			if(level->portal->sprite_g.visible)
				level->player_hud.show_portal_text();

			if (game.input.key_pressed(SDL_SCANCODE_Z) && !level->player_hud.is_showing_text_box() && !npc)
			{
				game._game_state.set_state(GAME_STATE_CREDITS);
			}
		}

		_ent_buff.clear();
		if (sword_collider.check_intersection(sword_collider.position, "gate", _ent_buff))
		{
			auto gate = (Gate*)_ent_buff[0]->parent;
			
			if (game.input.key_pressed(SDL_SCANCODE_Z) && level->player_hud.key_g.visible)
			{
				gate->open();
			}
		}

		if (position.x + origin.x < 0)
			position.x = 0 - origin.x;
		if (position.y + origin.y < 0)
			position.y = 0 - origin.y;
		if (position.x + origin.x + size.x > level->size.x)
			position.x = level->size.x - (origin.x + size.x);
		if (position.y + origin.y + size.y > level->size.y)
			position.y = level->size.y - (origin.y + size.y);

		_health_timer += dt;
		if (_health_timer >= 3.0f)
		{
			++health;
			_health_timer = 0.0f;

			if (health > 3)
				health = 3;
		}

		level->player_hud.set_hearts(health);
	}

	void Player::attack()
	{
		/*
		_ent_buff.clear();
		if (sword_collider.check_intersection(sword_collider.position, "tree", _ent_buff))
		{
			auto tree = _ent_buff[0]->parent;
			world->remove_entity(tree);
		}
		*/

		_ent_buff.clear();
		if (sword_collider.check_intersection(sword_collider.position, "monster", _ent_buff))
		{
			for (auto e : _ent_buff)
			{
				auto m = (Monster*)e;
				m->take_hit();
			}
		}
	}

	void Player::take_hit()
	{
		health--;
		_health_timer = 0.0f;

		_red_flash.hit();

		auto blood = new BloodSquirt(&level->blood_squirt_tex);
		blood->emit_pos = get_center_pos();
		blood->set_layer(get_layer() + 1);
		world->add_entity(blood);

		if (health <= 0)
		{
			health = 0;

			die();
		}
	}

	void Player::die()
	{
		world->remove_entity(this);
	}
}