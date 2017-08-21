#include "player_hud.h"
#include "level.h"
#include "player.h"
#include "npc.h"

#include "../world.h"
#include "../cam_ent.h"

#include "../game.h"

namespace jaw
{
	const int PlayerHud::LAYER_NUM = 1000000;

	PlayerHud::PlayerHud()
	{
		world = nullptr;
		level = nullptr;

		_heart_count = 3;

		_cur_dialogue = nullptr;
		_dialogue_index = 0;
	}

	void PlayerHud::init(Level* level)
	{
		entity.graphic = &group;
		entity.set_layer(LAYER_NUM);

		Bitmap heart_bmp;
		heart_bmp.create("../assets/heart.png");
		heart_tex.create(heart_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		hearts_g[0].create(&heart_tex);
		hearts_g[1] = hearts_g[0];
		hearts_g[2] = hearts_g[0];

		for (int i = 0; i < 3; ++i)
		{
			hearts_g[i].position = Point(50 + i * 20, 20);
			group.add(&hearts_g[i]);
		}

		heart_text_g.create(&level->font);
		heart_text_g.set_text("Life:");
		heart_text_g.set_scale(0.5f, 0.5f);
		heart_text_g.position = { 20, 20 };

		group.add(&heart_text_g);

		to_talk_text.create(&level->font);
		to_talk_text.set_text("Press 'Z' to talk...");
		to_talk_text.set_scale(0.5f, 0.5f);
		to_talk_text.visible = false;

		group.add(&to_talk_text);

		Bitmap text_box_bmp;
		text_box_bmp.create("../assets/text_box.png");
		text_box_tex.create(text_box_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		text_box_sprite.create(&text_box_tex);
		text_box_sprite.position = { 0, 300 - 74 };
		text_box_sprite.visible = false;

		group.add(&text_box_sprite);

		text_box_text.create(&level->font);
		text_box_text.set_scale(0.5f, 0.5f);
		text_box_text.set_word_wrap(true);
		text_box_text.set_word_wrap_width(380);
		text_box_text.position = { 0 + 10, 300 - 74 + 10 };
		text_box_text.color = vcm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
		text_box_text.set_text("This is only a test.");
		text_box_text.visible = false;

		group.add(&text_box_text);		

		Bitmap button_presser_bmp;
		button_presser_bmp.create("../assets/text_box_button_presser.png");
		button_presser_tex.create(button_presser_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		button_presser_sprite.create(&button_presser_tex);
		button_presser_sprite.frame_size = { 16, 16 };
		button_presser_sprite.position = { 316, 300 - 24 };

		SpriteAnim anim = { "idle", { 0, 1, 2, 3 }, 8, true };
		button_presser_sprite.add_anim(anim);
		button_presser_sprite.play_anim("idle");
		button_presser_sprite.visible = false;

		group.add(&button_presser_sprite);

		button_presser_text.create(&level->font);
		button_presser_text.set_scale(0.5f, 0.5f);
		button_presser_text.color = { 0.0f, 0.0f, 0.0f, 1.0f };
		button_presser_text.set_text("Press 'Z'...");
		button_presser_text.position = { 316 + 16, 300 - 24 };
		button_presser_text.visible = false;

		group.add(&button_presser_text);

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

		entity.parent = world->cam_ent;
		world->add_entity(&entity);
	}

	void PlayerHud::remove()
	{
		entity.parent = nullptr;
		world->remove_entity(&entity);

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
			to_talk_text.position = level->player->npc->position + Point(-10, 0) - world->cam_ent->position;
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
			to_talk_text.position = level->player->npc->position + Point(-10, 0) - world->cam_ent->position;
		}

		if (_cur_dialogue)
		{
			if (game.input.key_pressed(SDL_SCANCODE_Z))
			{
				_dialogue_index++;

				if (_dialogue_index >= (int)_cur_dialogue->list.size())
				{
					hide_text_box();
				}
				else
				{
					text_box_text.set_text(_cur_dialogue->list[_dialogue_index]);
				}
			}
		}
	}

	void PlayerHud::show_text_box(DialogueList* dialogue)
	{
		if (_cur_dialogue != dialogue)
		{
			_cur_dialogue = dialogue;

			text_box_sprite.visible = true;
			text_box_text.visible = true;
			button_presser_sprite.visible = true;
			button_presser_text.visible = true;

			text_box_text.set_text(_cur_dialogue->list[0]);
		}
	}

	void PlayerHud::hide_text_box()
	{
		if (_cur_dialogue)
		{
			_cur_dialogue = nullptr;
			_dialogue_index = 0;

			text_box_sprite.visible = false;
			text_box_text.visible = false;
			button_presser_sprite.visible = false;
			button_presser_text.visible = false;
		}
	}
}