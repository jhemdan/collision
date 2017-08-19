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

		_cur_tree = nullptr;
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
		heart_text_g.set_scale(0.5f, 0.5f);
		heart_text_ent.rel_position = Point(20, 20);

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].graphic = &hearts_g[i];

			hearts[i].set_layer(LAYER_NUM);
		
			hearts[i].rel_position = Point(50 + i * 20, 20);
		}

		to_talk_text.create(&level->font);
		to_talk_text.set_text("Press 'Z' to talk...");
		to_talk_text.set_scale(0.5f, 0.5f);
		to_talk_ent.graphic = &to_talk_text;
		to_talk_ent.set_layer(LAYER_NUM);

		to_talk_text.visible = false;

		Bitmap text_box_bmp;
		text_box_bmp.create("../assets/text_box.png");
		text_box_tex.create(text_box_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		text_box_sprite.create(&text_box_tex);
		text_box_ent.graphic = &text_box_sprite;
		text_box_ent.rel_position = { 0, 300 - 74 };
		text_box_ent.set_layer(LAYER_NUM);

		text_box_text.create(&level->font);
		text_box_text.set_scale(0.5f, 0.5f);
		text_box_text.set_word_wrap(true);
		text_box_text.set_word_wrap_width(380);

		text_box_text_ent.graphic = &text_box_text;

		text_box_text_ent.parent = &text_box_ent;
		text_box_text_ent.rel_position = { 10, 10 };
		text_box_text_ent.set_layer(LAYER_NUM + 1);

		text_box_text.color = vcm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
		text_box_text.set_text("This is only a test.");

		text_box_sprite.visible = false;
		text_box_text.visible = false;

		Bitmap button_presser_bmp;
		button_presser_bmp.create("../assets/text_box_button_presser.png");
		button_presser_tex.create(button_presser_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		button_presser_sprite.create(&button_presser_tex);
		button_presser_sprite.frame_size = { 16, 16 };

		SpriteAnim anim = { "idle", { 0, 1, 2, 3 }, 8, true };
		button_presser_sprite.add_anim(anim);
		button_presser_sprite.play_anim("idle");

		button_presser_ent.graphic = &button_presser_sprite;
		button_presser_ent.rel_position = { 316, 300 - 24 };
		button_presser_ent.set_layer(LAYER_NUM + 1);

		button_presser_text.create(&level->font);
		button_presser_text.set_scale(0.5f, 0.5f);
		button_presser_text.color = { 0.0f, 0.0f, 0.0f, 1.0f };
		button_presser_text.set_text("Press 'Z'...");
		
		button_presser_text_ent.graphic = &button_presser_text;
		button_presser_text_ent.set_layer(LAYER_NUM + 1);
		button_presser_text_ent.rel_position = { 316 + 20, 300 - 24 };

		button_presser_text.visible = false;
		button_presser_sprite.visible = false;

		this->level = level;
	}

	void PlayerHud::clean()
	{
		heart_tex.destroy();

		text_box_tex.destroy();
	}

	void PlayerHud::add(World* w)
	{
		world = w;

		heart_text_ent.parent = world->cam_ent;

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].parent = world->cam_ent;
		}

		text_box_ent.parent = world->cam_ent;

		button_presser_ent.parent = world->cam_ent;
		button_presser_text_ent.parent = world->cam_ent;

		for (int i = 0; i < 3; ++i)
		{
			world->add_entity(&hearts[i]);
		}

		world->add_entity(&heart_text_ent);

		world->add_entity(&to_talk_ent);

		world->add_entity(&text_box_ent);

		world->add_entity(&text_box_text_ent);

		world->add_entity(&button_presser_ent);
		world->add_entity(&button_presser_text_ent);
	}

	void PlayerHud::remove()
	{
		heart_text_ent.parent = nullptr;

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].parent = nullptr;
		}

		text_box_ent.parent = nullptr;

		button_presser_ent.parent = nullptr;
		button_presser_text_ent.parent = nullptr;

		for (int i = 0; i < 3; ++i)
		{
			world->remove_entity(&hearts[i]);
		}

		world->remove_entity(&heart_text_ent);

		world->remove_entity(&to_talk_ent);

		world->remove_entity(&text_box_ent);

		world->remove_entity(&text_box_text_ent);

		world->remove_entity(&button_presser_ent);
		world->remove_entity(&button_presser_text_ent);

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

		if (_cur_tree)
		{
			if (_cur_tree->yes_no)
			{

			}
		}
	}

	void PlayerHud::show_text_box(TextBoxTree* tree)
	{
		if (_cur_tree != tree)
		{
			_cur_tree = tree;

			text_box_sprite.visible = true;
			text_box_text.visible = true;
			button_presser_sprite.visible = true;
			button_presser_text.visible = true;

			text_box_text.set_text(_cur_tree->msg);
		}
	}

	void PlayerHud::hide_text_box()
	{
		if (_cur_tree)
		{
			_cur_tree = nullptr;

			text_box_sprite.visible = false;
			text_box_text.visible = false;
			button_presser_sprite.visible = false;
			button_presser_text.visible = false;
		}
	}
}