#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"
#include "red_flash.h"

#include <vector>

namespace jaw
{
	struct Level;
	struct NPC;

	struct Player : Entity
	{
		static const float SPEED;
		static const float CAM_SPEED;

		static const float SWORD_COLLIDER_SIZE_X_LR;
		static const float SWORD_COLLIDER_SIZE_X_UD;

		static const float ATTACK_COOLDOWN;

		Point dir;
		SpriteGraphic sprite_g;

		Entity sword_ent;
		Texture2d* sword_tex;
		SpriteGraphic sword_g;

		Entity magenta_ent;
		Texture2d* magenta_tex;
		SpriteGraphic magenta_g;

		float _cool_down;

		CurDir cur_dir;

		vcm::vec2 cam_posv;

		Level* level;

		bool swinging;

		Entity sword_collider;
		float attack_timer;
		const float slash_moment;
		bool has_attacked;

		int health;

		RedFlash _red_flash;

		std::vector<Entity*> _ent_buff;

		NPC* npc;

		Player(Texture2d* tex, Level* level);
		~Player();

		Point get_cam_dest_pos() const;		

		void on_added() override;
		void on_removed() override;

		void update(float dt) override;

		void attack();

		void take_hit();
		void die();
	};
}