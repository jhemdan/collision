#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"

namespace jaw
{
	struct Level;

	struct Player : Entity
	{
		Point dir;
		SpriteGraphic sprite_g;

		Entity sword_ent;
		Texture2d* sword_tex;
		SpriteGraphic sword_g;

		Entity magenta_ent;
		Texture2d* magenta_tex;
		SpriteGraphic magenta_g;

		const float SPEED = 100.0f;
		const float CAM_SPEED = 8.0f;

		const float SWORD_COLLIDER_SIZE_X_LR = 0.8f;
		const float SWORD_COLLIDER_SIZE_X_UD = 0.2f;

		const float ATTACK_COOLDOWN = .69f;

		float _cool_down;

		CurDir cur_dir;

		vcm::vec2 cam_posv;

		Level* level;

		bool swinging;

		Entity sword_collider;
		float attack_timer;
		const float slash_moment;
		bool has_attacked;

		Player(Texture2d* tex, Level* level);
		~Player();

		Point get_cam_dest_pos() const;		

		void on_added() override;
		void on_removed() override;

		void update(float dt) override;

		void attack();
	};
}