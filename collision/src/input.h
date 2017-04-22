#pragma once

#include <SDL2/SDL_scancode.h>

namespace jaw
{
	struct Input
	{
		Input();

		void flush_keys(); //called before event polling
		void on_key_change(SDL_Scancode k, bool state);

		bool key_is_down(SDL_Scancode k) const;
		bool key_pressed(SDL_Scancode k) const;
		bool key_released(SDL_Scancode k) const;

		bool keys[SDL_NUM_SCANCODES];
		bool pkeys[SDL_NUM_SCANCODES];

		SDL_Scancode _changed[1024];
		int _num_changed;
	};
}