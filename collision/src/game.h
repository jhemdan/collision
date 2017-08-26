#pragma once

#include "texture_2d.h"
#include "renderer.h"
#include "world.h"
#include "sprite_graphic.h"
#include "input.h"
#include "specific/game_state.h"

#include <vecmath/matrix.hpp>

struct SDL_Window;

namespace jaw
{
	struct Level;

	struct Game
	{
		Game();

		void set_initial_vars(SDL_Window* window, bool running);

		//throws exception on failure
		void init();
		void clean();

		void update(float dt);
		void render();
		
		SDL_Window* window;
		bool running;

		//used for frames per second calculation
		float timer;
		float total_fps;
		float fps_ticks;

		Input input;

		vcm::mat4 cam_proj; //camera projection matrix
		vcm::mat4 cam_view; //camera view matrix
		vcm::mat4 inv_cam_view; //inverse camera view matrix

		Renderer renderer;

		World world;

		GameState _game_state;
	};
	
	extern Game game;
}