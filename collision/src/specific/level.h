#pragma once

#include "../entity.h"
#include "../tilemap_graphic.h"
#include "../texture_2d.h"
#include "../sprite_graphic.h"
#include "player_hud.h"
#include "../font.h"

#include <vector>

namespace jaw
{
	struct Player;

	struct Level : Entity
	{
		Entity tilemap_ent;
		TilemapGraphic tilemap_graphic;
		Texture2d tilemap_tex;

		Texture2d tree_tex;
		SpriteGraphic weeds1_g;
		Texture2d weeds1_tex;
		Texture2d player_tex;
		Texture2d monster_tex;

		Font font;

		std::vector<Entity*> ents;

		Player* player;

		Point size;

		PlayerHud player_hud;

		Level();

		void load();
		void clean();

		void on_added() override;
		void on_removed() override;
	};
}