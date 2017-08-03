#pragma once

#include "../sprite_graphic.h"
#include "../text_graphic.h"
#include "../entity.h"

namespace jaw
{
	struct World;

	struct PlayerHud
	{
		const int LAYER_NUM = 1000000;

		Texture2d heart_tex;
		SpriteGraphic hearts_g[3];
		Entity hearts[3];
		TextGraphic heart_text_g;
		Entity heart_text_ent;

		int _heart_count;

		World* world;

		PlayerHud();

		void init();
		void clean();
		void add(World* w);
		void remove();

		void set_hearts(int hc);
	};
}