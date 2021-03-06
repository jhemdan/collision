#pragma once

#include "../entity.h"
#include "../tilemap_graphic.h"
#include "../texture_2d.h"
#include "../sprite_graphic.h"
#include "player_hud.h"
#include "../font.h"
#include "../sound_buffer.h"
#include "../sound_source.h"

#include <vector>

namespace jaw
{
	struct Player;
	struct Portal;

	struct Level : Entity
	{
		Entity tilemap_ent;
		TilemapGraphic tilemap_graphic;
		Texture2d tilemap_tex;

		Texture2d tree_tex;
		SpriteGraphic weeds1_g;
		Texture2d weeds1_tex;
		Texture2d player_tex;
		Texture2d npc_tex;
		Texture2d monster_tex;
		Texture2d monster_flame_tex;
		Texture2d smoke_tex;
		Texture2d gate_tex;
		Texture2d bush_tex;
		Texture2d blood_squirt_tex;
		Texture2d portal_tex;

		SoundBuffer bg_music_buff;
		SoundSource bg_music_src;

		Font font;

		std::vector<Entity*> ents;

		Player* player;
		Portal* portal;

		Point size;

		PlayerHud player_hud;

		Level();

		void load();
		void clean();

		void on_added() override;
		void on_removed() override;

		void update(float dt) override;
	};
}