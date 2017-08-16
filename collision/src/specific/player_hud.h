#pragma once

#include "../sprite_graphic.h"
#include "../text_graphic.h"
#include "../entity.h"

namespace jaw
{
	struct Level;
	struct World;

	struct PlayerHud
	{
		static const int LAYER_NUM;

		Texture2d heart_tex;
		SpriteGraphic hearts_g[3];
		Entity hearts[3];
		TextGraphic heart_text_g;
		Entity heart_text_ent;

		Entity to_talk_ent;
		TextGraphic to_talk_text;

		int _heart_count;

		World* world;
		Level* level;

		PlayerHud();

		void init(Level* level);
		void clean();
		void add(World* w);
		void remove();

		void update(float dt);

		void set_hearts(int hc);

		void show_to_talk_text();
		void hide_to_talk_text();
	};
}