#include "game.h"
#include "log.h"
#include "entity.h"
#include "wav_file.h"
#include "sound_buffer.h"
#include "sound_source.h"
#include "tilemap_graphic.h"
#include "exception.h"
#include "cam_ent.h"
#include "font.h"
#include "tinyxml2err.h"
#include "text_graphic.h"
#include "particle_graphic.h"

#include "specific/level.h"

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vecmath/pi.hpp>
#include <vector>

#include <tinyxml2.h>
#include <map>

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

		/*
		Bitmap test_rgba;
		test_rgba.create("../assets/main_character.png");

		sprite_texture.create(test_rgba, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_REPEAT);

		sprite_graphic.create(&sprite_texture);

		auto font = new Font();
		font->create("../assets/main_font.fnt");

		auto text = new TextGraphic();
		text->create(font);
		text->scale = 0.5f;
		text->set_text(
			"The quick brown fox jumps over the lazy dog.\n"
			"Why did the quick brown fox jump over the lazy dog?\n"
			"Because the lazy dog was too lazy.\n\n"
		);

		auto text_ent = new Entity();
		text_ent->graphic = text;

		text_ent->set_layer(10000000);

		world.add_entity(text_ent);

		Bitmap particle_bmp;
		particle_bmp.create("../assets/test_particles.png");
		Texture2d* particle_tex = new Texture2d();
		particle_tex->create(particle_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		auto test_particles = new ParticleGraphic();
		test_particles->create(particle_tex);

		test_particles->frame_size = { 16, 16 };

		ParticleAnim test_anim;
		test_anim.name = "test";
		test_anim.frames = { 0, 1, 2, 3 };
		test_particles->add_anim(test_anim);
		test_particles->method.anim = "test";

		test_particles->method.spawn_radius_max = 100;

		auto test_particle_ent = new Entity();
		test_particle_ent->graphic = test_particles;
		test_particle_ent->set_layer(text_ent->get_layer() + 1);
		test_particle_ent->position = { 75, 75 };

		world.add_entity(test_particle_ent);
		*/

		//move all this specific stuff (level, player, etc.) into an organized space (different files, etc.)
		//make ibrahim the main character

		/*
		level = new Level();
		level->load();
		world.add_entity(level);
		*/

		//TODO get sound to stop playing after destroyed

		_game_state.load(&world);
		_game_state.set_state(GAME_STATE_MENU);
	}

	void Game::clean()
	{
		world.clear();

		/*
		level->clean();
		delete level;
		*/

		_game_state.clean();

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