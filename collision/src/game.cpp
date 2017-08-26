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
		//enable blending so transparent images appear transparent
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//vsycn (0 = no vsync, 1 = yes vsync)
		SDL_GL_SetSwapInterval(1);

		//orthographic camera projection matrix
		cam_proj = vcm::orthographic(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

		renderer.init();

		_game_state.load(&world);
		_game_state.set_state(GAME_STATE_MENU);
	}

	void Game::clean()
	{
		world.clear();

		_game_state.clean();

		renderer.clean();
	}

	void Game::update(float dt)
	{
		//calculate frames per second averages every second
		//and display FPS
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