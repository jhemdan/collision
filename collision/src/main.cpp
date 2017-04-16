#include "log.h"
#include "load_file.h"
#include "shader.h"
#include "bitmap.h"
#include "texture_2d.h"
#include "mesh_buffers.h"

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <chrono>

namespace jaw
{
	struct Game
	{
		Game();

		void set_initial_vars(SDL_Window* window, bool running);
		void init();

		void update(float dt);
		void render();

		SDL_Window* window;
		bool running;

		float timer;
		float total_fps;
		float fps_ticks;
	};

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

		Shader sprite_shader;
		sprite_shader.create("../assets/sprite.vert", "../assets/sprite.frag", { {"vertex", 0}, {"uv", 1} });
	
		Bitmap test_rgba;
		test_rgba.create("../assets/test_rgba.png");

		Texture2d test_tex;
		test_tex.create(test_rgba, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_REPEAT);

		Mesh mesh;
		mesh.vertices =
		{
			vcm::vec2{0.0f, 0.0f},
			vcm::vec2{0.0f, 1.0f},
			vcm::vec2{1.0f, 1.0f},
			vcm::vec2{1.0f, 0.0f}
		};

		mesh.triangles =
		{
			{0, 1, 2},
			{2, 3, 0}
		};

		MeshBuffers mesh_buffers;
		mesh_buffers.create(mesh);
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
	}

	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}
}

int main(int argc, char** argv)
{
	int status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	assert(!status);

	status = IMG_Init(IMG_INIT_PNG);
	assert(status == IMG_INIT_PNG);

	const char* title = "Collision";
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

	status = (int)jaw::init_log("log.txt");
	assert(status);

	std::string test_file;
	status = jaw::load_file("../assets/test_file.txt", test_file);
	assert(status);

	jaw::Game game;

	game.set_initial_vars(window, true);

	game.init();

	auto start_time = std::chrono::system_clock::now();
	auto end_time = std::chrono::system_clock::now();
	float dt = 1.0f / 60.0f;

	while (game.running)
	{
		start_time = std::chrono::system_clock::now();

		game.update(dt);
		game.render();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				game.running = false;
				break;
			}
		}

		end_time = std::chrono::system_clock::now();
		auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
		dt = ns / 1000000000.0f;
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}