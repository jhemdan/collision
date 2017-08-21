#pragma once

#include "../sprite_graphic.h"
#include "../text_graphic.h"
#include "../entity.h"
#include "dialogue_list.h"

#include "../graphic_group.h"

namespace jaw
{
	struct Level;
	struct World;

	struct PlayerHud
	{
		static const int LAYER_NUM;

		Entity entity;
		GraphicGroup group;

		Texture2d heart_tex;
		SpriteGraphic hearts_g[3];
		TextGraphic heart_text_g;

		TextGraphic to_talk_text;

		Texture2d text_box_tex;
		SpriteGraphic text_box_sprite;
		TextGraphic text_box_text;

		Texture2d button_presser_tex;
		SpriteGraphic button_presser_sprite;
		TextGraphic button_presser_text;

		Texture2d key_tex;
		SpriteGraphic key_g;

		DialogueList* _cur_dialogue;
		int _dialogue_index;

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

		void show_text_box(DialogueList* dialogue);
		void hide_text_box();

		bool is_showing_text_box() const { return _cur_dialogue != nullptr; }
	};
}