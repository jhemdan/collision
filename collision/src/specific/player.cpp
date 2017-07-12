#include "player.h"

#include "../world.h"
#include "../game.h"
#include "level.h"
#include "../cam_ent.h"

namespace jaw
{
	Player::Player(Texture2d* tex, Level* level)
		: sword_tex(nullptr)
		, magenta_tex(nullptr)

		, attack_timer(0.0f)
		, slash_moment(1 / 14.0f)
		, has_attacked(false)
	{
		sprite_g.create(tex);

		graphic = &sprite_g;

		size = { 14, 23 };
		origin = { 25, 41 };

		position = { 0, 0 };

		sprite_g.set_clip_rect(Rect(0, 0, 64, 64));

		solid = true;

		sprite_g.frame_size = { 64, 64 };

		cur_dir = DOWN;

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

		SpriteAnim sword_slash_r;
		sword_slash_r.name = "slash_r";
		sword_slash_r.frames = { 0, 1, 2, 3 };
		sword_slash_r.fps = 14;
		sword_slash_r.loop = false;

		sword_g.add_anim(sword_slash_r);

		sword_ent.graphic = &sword_g;

		sword_g.visible = false;

		swinging = false;

		Bitmap magenta_bmp;
		magenta_bmp.create("../assets/magenta_box.png");
		magenta_tex = new Texture2d();
		magenta_tex->create(magenta_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		magenta_g.create(magenta_tex);
		magenta_g.scale.x = 0.8f;
		magenta_g.scale.y = 0.75f;

		magenta_ent.graphic = &magenta_g;

		sword_collider.parent = &magenta_ent;
		sword_collider.size = Point(magenta_tex->w, magenta_tex->h) * magenta_g.scale;
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

		switch (cur_dir)
		{
		case DOWN:
			cur_anim = "idle_down";
			break;
		case UP:
			cur_anim = "idle_up";
			break;
		case LEFT:
			cur_anim = "idle_left";
			break;
		case RIGHT:
			cur_anim = "idle_right";
			break;
		}

		dir = Point(0, 0);
		if (game.input.key_is_down(SDL_SCANCODE_RIGHT))
		{
			dir.x = 1;
			cur_anim = "right";
			cur_dir = RIGHT;
		}
		else if (game.input.key_is_down(SDL_SCANCODE_LEFT))
		{
			dir.x = -1;
			cur_anim = "left";
			cur_dir = LEFT;
		}
		if (game.input.key_is_down(SDL_SCANCODE_DOWN))
		{
			dir.y = 1;
			cur_anim = "down";
			cur_dir = DOWN;
		}
		else if (game.input.key_is_down(SDL_SCANCODE_UP))
		{
			dir.y = -1;
			cur_anim = "up";
			cur_dir = UP;
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

		if (!swinging && game.input.key_is_down(SDL_SCANCODE_X))
		{
			swinging = true;
			sword_g.play_anim("slash_r");
			sword_g.visible = true;
		}

		if (swinging)
		{
			if (sword_g.has_reached_end())
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

		sword_ent.position = position + Point(10, 0);
		sword_ent.set_layer(sword_ent.position.y + 64 + 1);

		magenta_ent.position = position + Point(43, 32);
		magenta_ent.set_layer(get_layer() + 1);
	}

	void Player::attack()
	{
		std::vector<Entity*> tree_boxes;
		if (sword_collider.check_intersection(sword_collider.position, "tree", tree_boxes))
		{
			auto tree = tree_boxes[0]->parent;
			world->remove_entity(tree);
		}
	}
}