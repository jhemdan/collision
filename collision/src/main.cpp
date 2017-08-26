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
	//open log file for logging
	try
	{
		jaw::init_log(LOG_FILE_NAME);
	}
	catch (const jaw::Exception&)
	{
		JAW_ASSERT_MSG(false, "Couldn't init log.");
	}

	//initialize sdl
	int status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	JAW_ASSERT_MSG(!status, "Bad status for SDL_Init");

	//initialize sdl_image for png image loading
	status = IMG_Init(IMG_INIT_PNG);
	JAW_ASSERT_MSG(status == IMG_INIT_PNG, "Bad status for IMG_Init");

	//setup window and opengl context
	const char* title = "A Side Quest - Dedicated to Ibrahim Hemdan";
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = x;
	int w = 800;
	int h = 600;
	Uint32 flags = SDL_WINDOW_OPENGL;

	auto window = SDL_CreateWindow(title, x, y, w, h, flags);
	auto context = SDL_GL_CreateContext(window);
	JAW_ASSERT_MSG(window && context, "Couldn't create window/OpenGL context.");

	//init GLEW for OpenGL extensions
	status = glewInit();
	JAW_ASSERT_MSG(status == GLEW_OK, "Bad status for glewInit()");

	//setup OpenAL for audio
	ALCdevice* al_device;
	ALCcontext* al_context;

	al_device = alcOpenDevice(nullptr);
	JAW_ASSERT_MSG(al_device, "Couldn't create al_device");

	al_context = alcCreateContext(al_device, nullptr);
	JAW_ASSERT_MSG(al_context, "Couldn't create al_context");

	alcMakeContextCurrent(al_context);

	//give Game struct information
	jaw::game.set_initial_vars(window, true);

	try
	{
		//initialize the game
		jaw::game.init();

		//used for frame time difference calculation
		auto start_time = std::chrono::system_clock::now();
		auto end_time = std::chrono::system_clock::now();
		float dt = 1.0f / 60.0f;

		//MAIN GAME LOOP
		//do this stuff while the game is running
		while (jaw::game.running)
		{
			//get the current time
			start_time = std::chrono::system_clock::now();

			//update and render game
			jaw::game.update(dt);
			jaw::game.render();

			//update the input so we know which keys were pressed/released
			jaw::game.input.flush_keys();

			//loop through SDL events
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

			//get the current time and get the difference from start_time
			end_time = std::chrono::system_clock::now();
			auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
			dt = ns / 1000000000.0f;
		}
	}
	catch (const jaw::Exception& e)
	{
		//show a messagebox if there's an exception
		//tell the user to read the log file for more info
		std::string msg = e.msg + "\n" + "See log file \"" + LOG_FILE_NAME + "\" for details.";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msg.c_str(), nullptr);
	}

	//clean up memory and stuff
	jaw::game.clean();

	//close OpenAL
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(al_context);
	alcCloseDevice(al_device);

	//destroy OpenGL context and window and shut down SDL + SDL_image
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	return 0;
}