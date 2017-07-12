#include "oak_tree.h"

#include "../world.h"

namespace jaw
{
	OakTree::OakTree(Texture2d* tex)
	{
		tree_g.create(tex);
		tree_g.origin = { 107, 198 };

		graphic = &tree_g;

		auto rects = 
		{
			Rect(88, 25, 36, 181),
			Rect(56, 39, 95, 109),
			Rect(150, 47, 27, 101),
			Rect(177, 62, 8, 77),
			Rect(185, 90, 7, 41),
			Rect(39, 53, 18, 95),
			Rect(24, 76, 15, 59)
		};

		for (auto r : rects)
		{
			auto box = new Entity();

			box->parent = this;
			box->solid = true;
			box->size = Point(r.w, r.h);
			box->rel_position = -tree_g.origin + Point(r.x, r.y);

			box->type = "tree";

			boxes.push_back(box);
		}
	}

	OakTree::~OakTree()
	{
		for (auto b : boxes)
			delete b;
	}

	void OakTree::on_added()
	{
		Entity::on_added();

		for (auto b : boxes)
		{
			world->add_entity(b);
		}
	}

	void OakTree::on_removed()
	{
		Entity::on_removed();

		for (auto b : boxes)
		{
			world->remove_entity(b);
		}
	}
}