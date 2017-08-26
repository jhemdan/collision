#include "gate.h"
#include "../world.h"

namespace jaw
{
	Gate::Gate(Texture2d* tex)
	{
		sprite_g.create(tex);

		graphic = &sprite_g;

		auto rects =
		{
			Rect(1, 67, 10, 61),
			Rect(117, 67, 10, 61),
			Rect(11, 79, 106, 47)
		};

		for (auto r : rects)
		{
			auto box = new Entity();

			box->parent = this;
			box->solid = true;
			box->size = Point(r.w, r.h);
			box->rel_position = Point(r.x, r.y);

			boxes.push_back(box);
		}

		boxes[2]->type = "gate";

		sprite_g.frame_size = { 128, 128 };

		SpriteAnim anim = { "closed",{ 0 }, 8, true };
		sprite_g.add_anim(anim);

		anim = { "open", { 1}, 8, true };
		sprite_g.add_anim(anim);

		sprite_g.play_anim("closed");
	}

	Gate::~Gate()
	{
		for (auto b : boxes)
			delete b;

		sprite_g.destroy();
	}

	void Gate::on_added()
	{
		Entity::on_added();

		for (auto b : boxes)
		{
			world->add_entity(b);
		}

		set_layer(position.y + 128);
	}

	void Gate::on_removed()
	{
		Entity::on_removed();

		for (auto b : boxes)
		{
			world->remove_entity(b);
		}
	}

	void Gate::open()
	{
		sprite_g.play_anim("open");
		boxes[2]->solid = false;
	}
}