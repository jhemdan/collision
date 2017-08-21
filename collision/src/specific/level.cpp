#include "level.h"

#include "../exception.h"
#include "../tinyxml2err.h"
#include "../world.h"
#include "player.h"
#include "oak_tree.h"
#include "monster.h"
#include "npc.h"
#include "gate.h"
#include "bush.h"

namespace jaw
{
	Level::Level()
		: player(nullptr)
	{

	}

	void Level::load()
	{
		std::string path = "../assets/level1.oel";

		Bitmap tilemap_bmp;
		tilemap_bmp.create("../assets/grass_tiles1.png");
		tilemap_tex.create(tilemap_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap tree_bmp;
		tree_bmp.create("../assets/oak_tree.png");
		tree_tex.create(tree_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap weeds1_bmp;
		weeds1_bmp.create("../assets/weeds1.png");
		weeds1_tex.create(weeds1_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		weeds1_g.create(&weeds1_tex);
		weeds1_g.origin = { 16, 31 };

		Bitmap player_bmp;
		player_bmp.create("../assets/ibrahim.png");
		player_tex.create(player_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap npc_bmp;
		npc_bmp.create("../assets/jawdat.png");
		npc_tex.create(npc_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		font.create("../assets/main_font.fnt");

		Bitmap monster_bmp;
		monster_bmp.create("../assets/monster.png");
		monster_tex.create(monster_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap monster_flame_bmp;
		monster_flame_bmp.create("../assets/flame.png");
		monster_flame_tex.create(monster_flame_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap gate_bmp;
		gate_bmp.create("../assets/gate.png");
		gate_tex.create(gate_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap bush_bmp;
		bush_bmp.create("../assets/bush.png");
		bush_tex.create(bush_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		struct Tile
		{
			int x, y;
			int id;
		};

		std::vector<Tile> tiles;

		tinyxml2::XMLDocument doc;

		auto err = doc.LoadFile(path.c_str());
		txml_check_err(err, path);

		int width = 0, height = 0;

		auto first_child = doc.FirstChild();
		if (first_child)
		{
			auto first_elem = first_child->ToElement();
			err = first_elem->QueryIntAttribute("width", &width);
			txml_check_err(err, path);

			err = first_elem->QueryIntAttribute("height", &height);
			txml_check_err(err, path);

			size.x = width;
			size.y = height;

			auto tileset_node = first_child->FirstChild();
			if (tileset_node)
			{
				auto first_tile = tileset_node->FirstChild();
				for (auto tile = first_tile; tile != nullptr; tile = tile->NextSibling())
				{
					auto tile_elem = tile->ToElement();

					Tile t;
					err = tile_elem->QueryIntAttribute("x", &t.x);
					txml_check_err(err, path);

					err = tile_elem->QueryIntAttribute("y", &t.y);
					txml_check_err(err, path);

					err = tile_elem->QueryIntAttribute("id", &t.id);
					txml_check_err(err, path);

					tiles.push_back(t);
				}
			}

			auto entities_node = tileset_node->NextSibling();
			if (entities_node)
			{
				auto add_oak_tree = [this](int x, int y)
				{
					auto e = new OakTree(&tree_tex);
					e->position = { x, y };

					e->set_layer(e->position.y);

					ents.push_back(e);
				};

				auto add_weeds1 = [this](int x, int y)
				{
					auto e = new Entity();
					e->position = { x, y };
					e->graphic = &weeds1_g;

					e->set_layer(e->position.y);

					ents.push_back(e);
				};

				auto add_player = [this](int x, int y)
				{
					if (!player)
					{
						auto e = new Player(&player_tex, this);
						player = e;
						e->position = { x, y };

						ents.push_back(e);
					}
				};

				auto add_npc = [this](int x, int y)
				{
					auto e = new NPC(&npc_tex, this);
					e->position = { x, y };

					ents.push_back(e);
				};

				auto add_monster = [this](int x, int y)
				{
					auto e = new Monster(&monster_tex, this);
					e->position = { x, y };

					ents.push_back(e);
				};

				auto add_gate = [this](int x, int y)
				{
					auto e = new Gate(&gate_tex);
					e->position = { x, y };

					ents.push_back(e);
				};

				auto add_bush = [this](int x, int y)
				{
					auto e = new Bush(&bush_tex);
					e->position = { x, y };
					e->set_layer(e->position.y);

					ents.push_back(e);
				};

				auto first_entity = entities_node->FirstChild();
				for (auto entity = first_entity; entity != nullptr; entity = entity->NextSibling())
				{
					auto ent_elem = entity->ToElement();

					std::string name;

					name = ent_elem->Name();

					int x, y;
					err = ent_elem->QueryIntAttribute("x", &x);
					txml_check_err(err, path);
					err = ent_elem->QueryIntAttribute("y", &y);
					txml_check_err(err, path);

					if (name == "oak_tree")
					{
						add_oak_tree(x, y);
					}
					else if (name == "weeds1")
					{
						add_weeds1(x, y);
					}
					else if (name == "player")
					{
						add_player(x, y);
					}
					else if (name == "jawdat")
					{
						add_npc(x, y);
					}
					else if (name == "monster")
					{
						add_monster(x, y);
					}
					else if (name == "gate")
					{
						add_gate(x, y);
					}
					else if (name == "bush")
					{
						add_bush(x, y);
					}
				}
			}
		}

		if (tiles.empty())
			throw Exception("No tiles in level file.");

		int cols = width / 32;
		int rows = height / 32;

		if (cols <= 0 || rows <= 0)
			throw Exception("Bad level size.");

		tilemap_graphic.create(&tilemap_tex, cols, rows, 32, 32);
		tilemap_ent.graphic = &tilemap_graphic;

		for (const auto& tile : tiles)
		{
			tilemap_graphic.set_tile(tile.x, tile.y, tile.id);
		}

		/*
		player = new Player(&player_tex, this);
		ents.push_back(player);

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				auto monster = new Monster(&monster_tex, this);
				ents.push_back(monster);

				monster->position = Point(200, 200) + Point{ i * 50, j * 50 };
			}
		}
		*/

		player_hud.init(this);

		auto test_text = new Entity();
		auto test_text_text = new TextGraphic();
		
		test_text_text->create(&font);
		test_text_text->set_text("1 2 3 4 5 6 7 8 9 This is a test for word-wrap. Testing, testing, 1 2 3. Testing, testing, 1 2 3. Is it working?");
		test_text_text->set_word_wrap(true);
		test_text_text->set_word_wrap_width(100);

		test_text_text->set_scale(0.5f, 0.5f);

		test_text->position = player->position;
		test_text->graphic = test_text_text;
		test_text->set_layer(player_hud.LAYER_NUM);

		ents.push_back(test_text);
	}

	void Level::clean()
	{
		player_hud.clean();

		for (auto e : ents)
		{
			delete e;
		}
		
		ents.clear();

		tilemap_tex.destroy();
		weeds1_tex.destroy();
		tree_tex.destroy();
		player_tex.destroy();
		npc_tex.destroy();
		monster_tex.destroy();
		monster_flame_tex.destroy();
		gate_tex.destroy();
		bush_tex.destroy();

		font.destroy();

		player = nullptr;
	}

	void Level::on_added()
	{
		Entity::on_added();

		world->add_entity(&tilemap_ent);

		for (auto e : ents)
			world->add_entity(e);

		player_hud.add(world);
	}

	void Level::on_removed()
	{
		Entity::on_removed();

		world->remove_entity(&tilemap_ent);

		for (auto e : ents)
			world->remove_entity(e);

		player_hud.remove();
	}

	void Level::update(float dt)
	{
		Entity::update(dt);

		player_hud.update(dt);
	}
}