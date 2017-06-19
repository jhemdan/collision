#include "game.h"
#include "log.h"
#include "entity.h"
#include "wav_file.h"
#include "sound_buffer.h"
#include "sound_source.h"
#include "tilemap_graphic.h"
#include "exception.h"
#include "cam_ent.h"

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vecmath/pi.hpp>
#include <vector>

#include <tinyxml2.h>

namespace jaw
{
	Game game;

	Game::Game()
	{
		window = nullptr;
		running = false;

		timer = 0.0f;
		total_fps = 0.0f;
		fps_ticks = 0.0f;
	}

	void Game::set_initial_vars(SDL_Window* window, bool running)
	{
		this->window = window;
		this->running = running;
	}

	void Game::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SDL_GL_SetSwapInterval(0);

		cam_proj = vcm::orthographic(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

		renderer.init();

		Bitmap test_rgba;
		test_rgba.create("../assets/main_character.png");

		sprite_texture.create(test_rgba, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_REPEAT);

		sprite_graphic.create(&sprite_texture);

		struct Level : Entity
		{
			Entity tilemap_ent;
			TilemapGraphic tilemap_graphic;
			Texture2d* tilemap_tex;

			SpriteGraphic tree_g;
			Texture2d* tree_tex;
			SpriteGraphic weeds1_g;
			Texture2d* weeds1_tex;

			std::vector<Entity*> ents;

			Point size;

			void load()
			{
				Bitmap tilemap_bmp;
				tilemap_bmp.create("../assets/grass_tiles1.png");
				tilemap_tex = new Texture2d();
				tilemap_tex->create(tilemap_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

				Bitmap tree_bmp;
				tree_bmp.create("../assets/oak_tree.png");
				tree_tex = new Texture2d();
				tree_tex->create(tree_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
				tree_g.create(tree_tex);
				tree_g.origin = { 107, 198 };

				Bitmap weeds1_bmp;
				weeds1_bmp.create("../assets/weeds1.png");
				weeds1_tex = new Texture2d();
				weeds1_tex->create(weeds1_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
				weeds1_g.create(weeds1_tex);
				weeds1_g.origin = { 16, 31 };

				struct ErrException : Exception
				{
					ErrException(tinyxml2::XMLError err)
					{
						msg = "TinyXML2 error: " + std::to_string((int)err);
					}
				};

				auto check_err = [](tinyxml2::XMLError err)
				{
					if (err)
						throw ErrException(err);
				};

				struct Tile
				{
					int x, y;
					int id;
				};

				std::vector<Tile> tiles;

				tinyxml2::XMLDocument doc;

				auto err = doc.LoadFile("../assets/test_level.oel");
				check_err(err);

				int width = 0, height = 0;

				auto first_child = doc.FirstChild();
				if (first_child)
				{
					auto first_elem = first_child->ToElement();
					err = first_elem->QueryIntAttribute("width", &width);
					check_err(err);

					err = first_elem->QueryIntAttribute("height", &height);
					check_err(err);

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
							check_err(err);

							err = tile_elem->QueryIntAttribute("y", &t.y);
							check_err(err);

							err = tile_elem->QueryIntAttribute("id", &t.id);
							check_err(err);

							tiles.push_back(t);
						}
					}

					auto entities_node = tileset_node->NextSibling();
					if (entities_node)
					{
						auto add_oak_tree = [this](int x, int y)
						{
							auto e = new Entity();
							e->position = { x, y };
							e->graphic = &tree_g;

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
							check_err(err);
							err = ent_elem->QueryIntAttribute("y", &y);
							check_err(err);

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
			}

			void on_added() override
			{
				Entity::on_added();

				world->add_entity(&tilemap_ent);

				for (auto e : ents)
					world->add_entity(e);
			}
		};

		auto level = new Level();
		level->load();
		world.add_entity(level);

		struct Player : Entity
		{
			Point dir;
			SpriteGraphic sprite_g;
			SpriteGraphic tree_g;

			const float SPEED = 100.0f;
			const float CAM_SPEED = 8.0f;

			vcm::vec2 cam_posv;

			Level* level;

			Player(Texture2d* tex, Level* level)
			{
				sprite_g.create(tex);

				graphic = &sprite_g;

				size = { 14, 23 };
				origin = { 25, 41 };

				position = { 0, 0 };

				auto tree_tex = new Texture2d();
				Bitmap tree_bmp;
				tree_bmp.create("../assets/oak_tree.png");
				tree_tex->create(tree_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_REPEAT);
				tree_g.create(tree_tex);

				sprite_g.set_clip_rect(Rect(0, 0, 64, 64));

				solid = true;

				sprite_g.frame_size = { 64, 64 };

				SpriteAnim anim = { "idle", { 0 }, 8, true };
				sprite_g.add_anim(anim);

				anim = { "down", { 1, 2, 3, 4, 5, 6 }, 11.5f, true };
				sprite_g.add_anim(anim);

				anim = { "left", { 25, 26, 27, 28, 29, 30 }, 11.5f, true };
				sprite_g.add_anim(anim);

				anim = { "right", { 17, 18, 19, 20, 21, 22 }, 11.5f, true };
				sprite_g.add_anim(anim);

				anim = { "up", { 9, 10, 11, 12, 13, 14 }, 11.5f, true };
				sprite_g.add_anim(anim);

				this->level = level;
			}

			Point get_cam_dest_pos() const
			{
				return position + Point(32, 32) - world->cam_ent->size * 0.5f;
			}

			~Player()
			{
				sprite_g.destroy();
			}

			void on_added() override
			{
				Entity::on_added();

				/*
				for (int i = 0; i < 4; ++i)
				{
					auto e = new Entity();
					
					/*
					auto tiles_g = new TilemapGraphic();
					Bitmap tiles_bmp;
					tiles_bmp.create("../assets/grass_tiles1.png");
					auto tiles_tex = new Texture2d();
					tiles_tex->create(tiles_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
					tiles_g->create(tiles_tex, 6, 6, 32, 32);

					for (int j = 0; j < 6 * 6; ++j)
						tiles_g->set_tile(j % 6, j / 6, rand() % 28);
					
					//e->graphic = tiles_g;
					e->graphic = &tree_g;

					e->size = { 49, 209 };
					e->origin = { -104, -47 };
					e->position = Point{ i % 2, i / 2 } *250 - Point{ 64, 64 };
					e->set_layer(e->position.y + e->size.y - e->origin.y - 11);
					e->solid = true;

					world->add_entity(e);

					auto box1 = new Entity();

					box1->position = e->position + Point{ 64, 47 };
					box1->size = { 124, 133 };
					box1->solid = true;

					world->add_entity(box1);

					auto box2 = new Entity();

					box2->position = e->position + Point{ 44, 71 };
					box2->size = { 164, 109 };
					box2->solid = true;

					world->add_entity(box2);

					auto box3 = new Entity();

					box3->position = e->position + Point{29, 90};
					box3->size = {200, 90};
					box3->solid = true;

					world->add_entity(box3);
				}
				*/

				cam_posv = (vcm::vec2)get_cam_dest_pos();
			}

			void update(float dt) override
			{
				Entity::update(dt);

				sprite_g.play_anim("idle");

				dir = Point(0, 0);
				if (game.input.key_is_down(SDL_SCANCODE_RIGHT))
				{
					dir.x = 1;
					sprite_g.play_anim("right");
				}
				else if (game.input.key_is_down(SDL_SCANCODE_LEFT))
				{
					dir.x = -1;
					sprite_g.play_anim("left");
				}
				if (game.input.key_is_down(SDL_SCANCODE_DOWN))
				{
					dir.y = 1;
					sprite_g.play_anim("down");
				}
				else if (game.input.key_is_down(SDL_SCANCODE_UP))
				{
					dir.y = -1;
					sprite_g.play_anim("up");
				}

				move(vcm::normalize((vcm::vec2)dir) * SPEED * dt);

				set_layer(position.y + origin.y + size.y);

				auto cam_dest_pos = get_cam_dest_pos();

				cam_posv += (vcm::vec2)(cam_dest_pos - cam_posv) * CAM_SPEED * dt;

				if (cam_posv.x < 0)
					cam_posv.x = 0;
				if (cam_posv.y < 0)
					cam_posv.y = 0;
				if (cam_posv.x + world->cam_ent->size.x > level->size.x)
					cam_posv.x = level->size.x - world->cam_ent->size.x;
				if (cam_posv.y + world->cam_ent->size.y > level->size.y)
					cam_posv.y = level->size.y - world->cam_ent->size.y;

				world->cam_ent->position = cam_posv;
			}
		};

		auto player = new Player(&sprite_texture, level);
		world.add_entity(player);

		world.cam_tran = vcm::mat4
		{
			{ 0.5f, 0, 0, 0 },
			{ 0, 0.5f, 0, 0 },
			{ 0, 0, 0.5f, 0 },
			{ 0, 0, 0, 1 }
		};
	}

	void Game::clean()
	{
		sprite_graphic.destroy();

		world.clear();

		sprite_texture.destroy();

		renderer.clean();
	}

	void Game::update(float dt)
	{
		timer += dt;
		total_fps += (1.0f / dt);
		fps_ticks++;

		if (timer >= 1.0f)
		{
			timer = 0.0f;

			std::cout << "FPS: " << (total_fps / fps_ticks) << std::endl;
			total_fps = fps_ticks = 0.0f;
		}

		world.update(dt);

		inv_cam_view = vcm::inverse(cam_view);
	}

	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		world.render(&renderer);

		SDL_GL_SwapWindow(window);
	}
}