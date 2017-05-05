#include "game.h"
#include "log.h"
#include "entity.h"
#include "wav_file.h"
#include "sound_buffer.h"
#include "sound_source.h"
#include "tilemap_graphic.h"

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

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

		cam_proj = vcm::orthographic(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

		renderer.init();

		Bitmap test_rgba;
		test_rgba.create("../assets/main_character.png");

		sprite_texture.create(test_rgba, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_REPEAT);

		sprite_graphic.create(&sprite_texture);

		struct Player : Entity
		{
			Point dir;
			SpriteGraphic sprite_g;
			SpriteGraphic tree_g;

			SoundBuffer* sound_buffer;
			SoundSource* sound_source;

			const float SPEED = 100.0f;

			Player(Texture2d* tex)
			{
				sprite_g.create(tex);

				graphic = &sprite_g;

				size = { 14, 23 };
				origin = { -25, -41 };

				position = { 160, 160 };

				auto tree_tex = new Texture2d();
				Bitmap tree_bmp;
				tree_bmp.create("../assets/oak_tree.png");
				tree_tex->create(tree_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_REPEAT);
				tree_g.create(tree_tex);

				sprite_g.set_clip_rect(Rect(0, 0, 64, 64));

				solid = true;

				sprite_g.frame_size = { 64, 64 };

				SpriteAnim anim = { "idle", { 0, 4, 8, 12 }, 8, true };
				sprite_g.add_anim(anim);

				anim = { "down", { 0 }, 8, true };
				sprite_g.add_anim(anim);

				anim = { "left", { 12 }, 8, true };
				sprite_g.add_anim(anim);

				anim = { "right", { 8 }, 8, true };
				sprite_g.add_anim(anim);

				anim = { "up", { 4 }, 8, true };
				sprite_g.add_anim(anim);

				WavFile test_wav;
				test_wav.create("../assets/test_song1.wav");

				sound_buffer = new SoundBuffer();
				sound_buffer->create(test_wav);

				sound_source = new SoundSource();
				sound_source->create();
				sound_source->queue_buffer(sound_buffer->id);
			}

			~Player()
			{
				sprite_g.destroy();
			}

			void on_added() override
			{
				Entity::on_added();

				for (int i = 0; i < 4; ++i)
				{
					auto e = new Entity();
					
					auto tiles_g = new TilemapGraphic();
					Bitmap tiles_bmp;
					tiles_bmp.create("../assets/grass_tiles1.png");
					auto tiles_tex = new Texture2d();
					tiles_tex->create(tiles_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);
					tiles_g->create(tiles_tex, 6, 6, 32, 32);

					for (int j = 0; j < 6 * 6; ++j)
						tiles_g->set_tile(j % 6, j / 6, rand() % 28);

					e->graphic = tiles_g;

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

				set_layer(position.y + size.y - origin.y);

				if (game.input.key_pressed(SDL_SCANCODE_SPACE))
				{
					sound_source->play();
				}
			}
		};

		auto player = new Player(&sprite_texture);
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