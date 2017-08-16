#include "player_hud.h"
#include "level.h"
#include "player.h"
#include "npc.h"

#include "../world.h"
#include "../cam_ent.h"

namespace jaw
{
	const int PlayerHud::LAYER_NUM = 1000000;

	PlayerHud::PlayerHud()
	{
		world = nullptr;
		level = nullptr;

		_heart_count = 3;
	}

	void PlayerHud::init(Level* level)
	{
		Bitmap heart_bmp;
		heart_bmp.create("../assets/heart.png");
		heart_tex.create(heart_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		hearts_g[0].create(&heart_tex);
		hearts_g[1] = hearts_g[0];
		hearts_g[2] = hearts_g[0];

		heart_text_g.create(&level->font);
		heart_text_ent.graphic = &heart_text_g;
		heart_text_ent.set_layer(LAYER_NUM);
		heart_text_g.set_text("Life:");
		heart_text_g.scale = vcm::vec2{ 0.5f };
		heart_text_ent.rel_position = Point(20, 20);

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].graphic = &hearts_g[i];

			hearts[i].set_layer(LAYER_NUM);
		
			hearts[i].rel_position = Point(50 + i * 20, 20);
		}

		to_talk_text.create(&level->font);
		to_talk_text.set_text("Press 'Z' to talk...");
		to_talk_text.scale = vcm::vec2{ .5f };
		to_talk_ent.graphic = &to_talk_text;
		to_talk_ent.set_layer(LAYER_NUM);

		to_talk_text.visible = false;

		this->level = level;
	}

	void PlayerHud::clean()
	{
		heart_tex.destroy();
	}

	void PlayerHud::add(World* w)
	{
		world = w;

		heart_text_ent.parent = world->cam_ent;

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].parent = world->cam_ent;
		}

		for (int i = 0; i < 3; ++i)
		{
			world->add_entity(&hearts[i]);
		}

		world->add_entity(&heart_text_ent);

		world->add_entity(&to_talk_ent);
	}

	void PlayerHud::remove()
	{
		heart_text_ent.parent = nullptr;

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].parent = nullptr;
		}

		for (int i = 0; i < 3; ++i)
		{
			world->remove_entity(&hearts[i]);
		}

		world->remove_entity(&heart_text_ent);

		world->remove_entity(&to_talk_ent);

		world = nullptr;
	}

	void PlayerHud::set_hearts(int hc)
	{
		if (hc < 0)
			hc = 0;
		if (hc > 3)
			hc = 3;

		if (_heart_count != hc)
		{
			_heart_count = hc;

			for (int i = 0; i < 3; ++i)
				hearts_g[i].visible = false;
			for (int i = 0; i < hc; ++i)
				hearts_g[i].visible = true;
		}
	}

	void PlayerHud::show_to_talk_text()
	{
		to_talk_text.visible = true;

		if (level->player->npc)
		{
			to_talk_ent.position = level->player->npc->position + Point(-10, 0);
		}
	}

	void PlayerHud::hide_to_talk_text()
	{
		to_talk_text.visible = false;
	}

	void PlayerHud::update(float dt)
	{
		if (level->player->npc)
		{
			to_talk_ent.position = level->player->npc->position + Point(-10, 0);
		}
	}
}