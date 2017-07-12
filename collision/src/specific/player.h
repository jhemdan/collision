#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"

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

		enum CurDir
		{
			DOWN, UP, LEFT, RIGHT
		};

		CurDir cur_dir;
		std::string cur_anim;

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