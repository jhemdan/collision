#include "menu.h"

#include "../world.h"
#include "../game.h"

namespace jaw
{
	Menu::Menu()
	{

	}

	void Menu::load()
	{
		font.create("../assets/main_font.fnt");

		entity.graphic = &group_g;

		top_text.create(&font);
		top_text.set_text("A Side Quest");
		top_text.position = { 200 - int(top_text.get_total_width() * 0.5f), 100 };

		group_g.add(&top_text);

		bottom_text.create(&font);
		bottom_text.set_scale(0.5f, 0.5f);
		bottom_text.set_text("Press 'Z' to play!");
		bottom_text.position = { 200 - (int)(bottom_text.get_total_width() * 0.5f), 150 };

		group_g.add(&bottom_text);
	}

	void Menu::clean()
	{
		font.destroy();
	}

	void Menu::on_added()
	{
		Entity::on_added();

		world->add_entity(&entity);
	}

	void Menu::on_removed()
	{
		Entity::on_removed();

		world->remove_entity(&entity);
	}

	void Menu::update(float dt)
	{
		Entity::update(dt);

		if (game.input.key_pressed(SDL_SCANCODE_Z))
		{
			game._game_state.set_state(GAME_STATE_LEVEL);
		}
	}
}