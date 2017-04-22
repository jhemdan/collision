#pragma once

#include "texture_2d.h"
#include "renderer.h"
#include "world.h"
#include "sprite_graphic.h"
#include "input.h"

#include <vecmath/matrix.hpp>

struct SDL_Window;

namespace jaw
{
	struct Game
	{
		Game();

		void set_initial_vars(SDL_Window* window, bool running);
		bool init();
		void clean();

		void update(float dt);
		void render();

		SDL_Window* window;
		bool running;

		float timer;
		float total_fps;
		float fps_ticks;

		Input input;

		vcm::mat4 cam_proj;
		vcm::mat4 cam_view;
		vcm::mat4 inv_cam_view;

		Texture2d sprite_texture;

		Renderer renderer;

		World world;

		SpriteGraphic sprite_graphic;
	};
	
	extern Game game;
}