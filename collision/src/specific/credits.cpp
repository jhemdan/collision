#include "credits.h"
#include "../world.h"
#include "../cam_ent.h"

namespace jaw
{
	Credits::Credits()
	{

	}

	void Credits::load()
	{
		font.create("../assets/main_font.fnt");

		entity.graphic = &group_g;

		top_text.create(&font);
		top_text.set_text("Created by Jawdat Hemdan");
		top_text.position = { 200 - int(top_text.get_total_width() * 0.5f), 100 };

		group_g.add(&top_text);

		bottom_text.create(&font);
		bottom_text.set_scale(0.5f, 0.5f);
		bottom_text.set_text("Dedicated to Ibrahim Hemdan");
		bottom_text.position = { 200 - (int)(top_text.get_total_width() * 0.5f), 150 };

		group_g.add(&bottom_text);

		WavFile wav_file;
		wav_file.create("../assets/jam2.wav");
		bg_music_buff.create(wav_file);
		bg_music_src.create();
		bg_music_src.queue_buffer(bg_music_buff.id);
	}

	void Credits::clean()
	{
		font.destroy();
		bg_music_src.destroy();
		bg_music_buff.destroy();
	}

	void Credits::on_added()
	{
		Entity::on_added();

		world->add_entity(&entity);

		bg_music_src.play();
	}

	void Credits::on_removed()
	{
		Entity::on_removed();

		world->remove_entity(&entity);
	}

	void Credits::update(float dt)
	{
		Entity::update(dt);

		world->cam_ent->position = {};
	}
}