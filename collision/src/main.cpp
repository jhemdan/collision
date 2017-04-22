#include "game.h"
#include "log.h"
#include "load_file.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <chrono>

int main(int argc, char** argv)
{
	int status = (int)jaw::init_log("log.txt");
	assert(status);

	std::string test_file;
	status = jaw::load_file("../assets/test_file.txt", test_file);
	assert(status);

	status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	assert(!status);

	status = IMG_Init(IMG_INIT_PNG);
	assert(status == IMG_INIT_PNG);

	const char* title = "A Side Quest";
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = x;
	int w = 800;
	int h = 600;
	Uint32 flags = SDL_WINDOW_OPENGL;

	auto window = SDL_CreateWindow(title, x, y, w, h, flags);
	auto context = SDL_GL_CreateContext(window);
	assert(window && context);

	status = glewInit();
	assert(status == GLEW_OK);

	jaw::game.set_initial_vars(window, true);

	bool b = jaw::game.init();
	assert(b);

	auto start_time = std::chrono::system_clock::now();
	auto end_time = std::chrono::system_clock::now();
	float dt = 1.0f / 60.0f;

	while (jaw::game.running)
	{
		start_time = std::chrono::system_clock::now();

		jaw::game.update(dt);
		jaw::game.render();

		jaw::game.input.flush_keys();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				jaw::game.running = false;
				break;

			case SDL_KEYDOWN:
				jaw::game.input.on_key_change(e.key.keysym.scancode, true);
				break;
			case SDL_KEYUP:
				jaw::game.input.on_key_change(e.key.keysym.scancode, false);
				break;
			}
		}

		end_time = std::chrono::system_clock::now();
		auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
		dt = ns / 1000000000.0f;
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	return 0;
}