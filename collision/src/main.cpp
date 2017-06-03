#include "game.h"
#include "log.h"
#include "load_file.h"
#include "exception.h"
#include "jaw_macros.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>

#include <al.h>
#include <alc.h>

static const char* LOG_FILE_NAME = "log.txt";

int main(int argc, char** argv)
{
	try
	{
		jaw::init_log(LOG_FILE_NAME);
	}
	catch (const jaw::Exception&)
	{
		JAW_ASSERT_MSG(false, "Couldn't init log.");
	}

	int status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	JAW_ASSERT_MSG(!status, "Bad status for SDL_Init");

	status = IMG_Init(IMG_INIT_PNG);
	JAW_ASSERT_MSG(status == IMG_INIT_PNG, "Bad status for IMG_Init");

	const char* title = "A Side Quest";
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = x;
	int w = 800;
	int h = 600;
	Uint32 flags = SDL_WINDOW_OPENGL;

	auto window = SDL_CreateWindow(title, x, y, w, h, flags);
	auto context = SDL_GL_CreateContext(window);
	JAW_ASSERT_MSG(window && context, "Couldn't create window/OpenGL context.");

	status = glewInit();
	JAW_ASSERT_MSG(status == GLEW_OK, "Bad status for glewInit()");

	ALCdevice* al_device;
	ALCcontext* al_context;

	al_device = alcOpenDevice(nullptr);
	JAW_ASSERT_MSG(al_device, "Couldn't create al_device");

	al_context = alcCreateContext(al_device, nullptr);
	JAW_ASSERT_MSG(al_context, "Couldn't create al_context");

	alcMakeContextCurrent(al_context);

	jaw::game.set_initial_vars(window, true);

	try
	{
		jaw::game.init();

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
	}
	catch (const jaw::Exception& e)
	{
		std::string msg = e.msg + "\n" + "See log file \"" + LOG_FILE_NAME + "\" for details.";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msg.c_str(), nullptr);
	}

	jaw::game.clean();

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(al_context);
	alcCloseDevice(al_device);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	return 0;
}