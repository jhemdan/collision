#pragma once

namespace jaw
{
	enum GameStateType
	{
		GAME_STATE_NONE,
		GAME_STATE_MENU,
		GAME_STATE_LEVEL,
		GAME_STATE_CREDITS
	};

	struct Menu;
	struct Level;
	struct Credits;
	struct World;

	struct GameState
	{
		GameStateType _type;

		World* _world;
		Menu* _menu;
		Level* _level;
		Credits* _credits;

		GameState();
		~GameState();

		void load(World* world);
		void clean();

		void set_state(GameStateType t);
	};
}