#include "game_state.h"
#include "../world.h"
#include "menu.h"
#include "level.h"
#include "credits.h"

namespace jaw
{
	GameState::GameState()
	{
		_type = GAME_STATE_NONE;

		_world = nullptr;

		_menu = new Menu();
		_level = new Level();
		_credits = new Credits();
	}

	GameState::~GameState()
	{
		delete _menu;
		delete _level;
		delete _credits;
	}

	void GameState::load(World* world)
	{
		_world = world;

		_menu->load();
		_level->load();
		_credits->load();
	}

	void GameState::clean()
	{
		_menu->clean();
		_level->clean();
		_credits->clean();
	}

	void GameState::set_state(GameStateType t)
	{
		if (_type != t)
		{
			switch (_type)
			{
			case GAME_STATE_MENU:
				_world->remove_entity(_menu);
				break;
			case GAME_STATE_LEVEL:
				_world->remove_entity(_level);
				break;
			case GAME_STATE_CREDITS:
				_world->remove_entity(_credits);
				break;
			}
			
			_type = t;

			switch (_type)
			{
			case GAME_STATE_NONE:
				break;
			case GAME_STATE_MENU:
				_world->add_entity(_menu);
				break;
			case GAME_STATE_LEVEL:
				_world->add_entity(_level);
				break;
			case GAME_STATE_CREDITS:
				_world->add_entity(_credits);
				break;
			}
		}
	}
}