#pragma once

#include "../entity.h"
#include "../sprite_graphic.h"
#include "cur_dir.h"
#include "idle_walking.h"
#include "text_box_tree.h"

namespace jaw
{
	struct Level;

	struct NPC : Entity
	{
		static const float SPEED;
		static const float STOP_TALKING_DIST;

		SpriteGraphic sprite_g;

		CurDir cur_dir;

		bool _in_idle;
		IdleWalking _idle_walking;
	
		Level* level;

		TextBoxTree* _text_tree;

		NPC(Texture2d* tex, Level* level);
		~NPC();

		void update(float dt) override;
		
		void _do_idle(float dt);
		void _do_talking(float dt);

		void talk();

		bool is_talking() const { return !_in_idle; }

		void _build_text_tree();
	};
}