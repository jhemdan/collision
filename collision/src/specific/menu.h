#pragma once

#include "../entity.h"
#include "../graphic_group.h"
#include "../font.h"
#include "../text_graphic.h"

namespace jaw
{
	struct Menu : Entity
	{
		Font font;

		Entity entity;
		GraphicGroup group_g;

		TextGraphic top_text;
		TextGraphic bottom_text;

		Menu();

		void load();
		void clean();

		void on_added() override;
		void on_removed() override;

		void update(float dt) override;
	};
}