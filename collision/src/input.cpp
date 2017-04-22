#include "input.h"

namespace jaw
{
	Input::Input()
	{
		for (auto& k : keys)
			k = false;
		for (auto& k : pkeys)
			k = false;

		_num_changed = 0;
	}

	void Input::flush_keys()
	{
		for (int i = 0; i < _num_changed; ++i)
		{
			pkeys[_changed[i]] = keys[_changed[i]];
		}

		_num_changed = 0;
	}

	void Input::on_key_change(SDL_Scancode k, bool state)
	{
		keys[k] = state;
		_changed[_num_changed++] = k;
	}

	bool Input::key_is_down(SDL_Scancode k) const
	{
		return keys[k];
	}

	bool Input::key_pressed(SDL_Scancode k) const
	{
		return keys[k] && !pkeys[k];
	}

	bool Input::key_released(SDL_Scancode k) const
	{
		return pkeys[k] && !keys[k];
	}
}