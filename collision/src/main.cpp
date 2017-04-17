#include "log.h"
#include "load_file.h"
#include "shader.h"
#include "bitmap.h"
#include "texture_2d.h"
#include "mesh_buffers.h"
#include "sprite.h"
#include "box.h"

#include <vecmath/matrix.hpp>
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
		void clean();

		void update(float dt);
		void render();

		SDL_Window* window;
		bool running;

		float timer;
		float total_fps;
		float fps_ticks;

		vcm::mat4 cam_proj;
		vcm::mat4 cam_view;
		vcm::mat4 mvp;

		Shader sprite_shader;
		Texture2d sprite_texture;

		std::vector<Sprite*> sprites;

		Sprite* sprite_a;
		Sprite* sprite_b;

		BoxWorld box_world;
		Box* box_a;
		Box* box_b;
	};

	Game::Game()
	{
		window = nullptr;
		running = false;

		timer = 0.0f;
		total_fps = 0.0f;
		fps_ticks = 0.0f;

		sprite_a = nullptr;
		sprite_b = nullptr;
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

		sprite_shader.create("../assets/sprite.vert", "../assets/sprite.frag", { { "position", 0 },{ "tex_coords", 1 },{ "color", 2 } });

		Bitmap test_rgba;
		test_rgba.create("../assets/test_rgba.png");

		sprite_texture.create(test_rgba, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_REPEAT);

		sprite_a = new Sprite();
		sprite_a->create(&sprite_texture);
		sprites.push_back(sprite_a);

		sprite_b = new Sprite();
		sprite_b->create(&sprite_texture);
		sprites.push_back(sprite_b);

		box_a = new Box();
		box_b = new Box();

		box_world.add_box(box_a);
		box_world.add_box(box_b);

		box_b->position = { 100, 100 };
	}

	void Game::clean()
	{
		box_world.clear();
		delete box_b;
		delete box_a;

		sprites.clear();
		sprite_b->destroy();
		delete sprite_b;
		sprite_a->destroy();
		delete sprite_a;

		sprite_texture.destroy();
		sprite_shader.destroy();
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

		mvp = cam_proj * vcm::inverse(cam_view);

		sprite_a->position = box_a->position;
		sprite_b->position = box_b->position;
	}

	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto sprite : sprites)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, sprite->texture->id);

			glUseProgram(sprite_shader.id);

			vcm::mat4 tran_mat = vcm::mat4
			{
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ sprite->position.x, sprite->position.y, 0, 1 }
			};

			vcm::mat4 temp_mvp = mvp * tran_mat;
			glUniformMatrix4fv(sprite_shader.get_uniform("mvp"), 1, GL_FALSE, &temp_mvp[0][0]);

			glUniform1i(sprite_shader.get_uniform("tex"), 0);
			glUniform2f(sprite_shader.get_uniform("quad_size"), (float)sprite->texture->w, (float)sprite->texture->h);

			glBindBuffer(GL_ARRAY_BUFFER, sprite->model.mbuffers.vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->model.mbuffers.ibo);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(2 * sizeof(float)));
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(4 * sizeof(float)));

			glDrawElements(GL_TRIANGLES, sprite->model.mbuffers.idata.size(), GL_UNSIGNED_INT, nullptr);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}

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

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	return 0;
}