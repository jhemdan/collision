#pragma once

#include "../entity.h"
#include "../graphic_group.h"
#include "../text_graphic.h"
#include "../font.h"
#include "../sound_source.h"
#include "../sound_buffer.h"

namespace jaw
{
	struct Credits : Entity
	{
		Font font;

		Entity entity;
		GraphicGroup group_g;

		TextGraphic top_text;
		TextGraphic bottom_text;

		SoundBuffer bg_music_buff;
		SoundSource bg_music_src;

		Credits();

		void load();
		void clean();

		void on_added() override;
		void on_removed() override;

		void update(float dt) override;
	};
}