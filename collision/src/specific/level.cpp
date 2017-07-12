#include "level.h"

#include "../exception.h"
#include "../tinyxml2err.h"
#include "../world.h"
#include "player.h"
#include "oak_tree.h"

namespace jaw
{
	Level::Level()
		: tilemap_tex(nullptr)
		, tree_tex(nullptr)
		, weeds1_tex(nullptr)
		, player_tex(nullptr)

		, player(nullptr)
	{

	}

	void Level::load()
	{
		std::string path = "../assets/test_level.oel";

		Bitmap tilemap_bmp;
		tilemap_bmp.create("../assets/grass_tiles1.png");
		tilemap_tex = new Texture2d();
		tilemap_tex->create(tilemap_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap tree_bmp;
		tree_bmp.create("../assets/oak_tree.png");
		tree_tex = new Texture2d();
		tree_tex->create(tree_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		Bitmap weeds1_bmp;
		weeds1_bmp.create("../assets/weeds1.png");
		weeds1_tex = new Texture2d();
		weeds1_tex->create(weeds1_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
		weeds1_g.create(weeds1_tex);
		weeds1_g.origin = { 16, 31 };

		Bitmap player_bmp;
		player_bmp.create("../assets/main_character.png");
		player_tex = new Texture2d();
		player_tex->create(player_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

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
					auto e = new OakTree(tree_tex);
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
				}
			}
		}

		if (tiles.empty())
			throw Exception("No tiles in level file.");

		int cols = width / 32;
		int rows = height / 32;

		if (cols <= 0 || rows <= 0)
			throw Exception("Bad level size.");

		tilemap_graphic.create(tilemap_tex, cols, rows, 32, 32);
		tilemap_ent.graphic = &tilemap_graphic;

		for (const auto& tile : tiles)
		{
			tilemap_graphic.set_tile(tile.x, tile.y, tile.id);
		}

		player = new Player(player_tex, this);
		ents.push_back(player);
	}

	void Level::clean()
	{
		for (auto e : ents)
		{
			delete e;
		}
		
		ents.clear();

		delete tilemap_tex;
		delete weeds1_tex;
		delete tree_tex;
		delete player_tex;

		tilemap_tex = nullptr;
		weeds1_tex = nullptr;
		tree_tex = nullptr;
		player_tex = nullptr;

		player = nullptr;
	}

	void Level::on_added()
	{
		Entity::on_added();

		world->add_entity(&tilemap_ent);

		for (auto e : ents)
			world->add_entity(e);
	}

	void Level::on_removed()
	{
		Entity::on_removed();

		world->remove_entity(&tilemap_ent);

		for (auto e : ents)
			world->remove_entity(e);
	}
}