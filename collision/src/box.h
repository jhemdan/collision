#pragma once

#include <vecmath/vector.hpp>
#include <vector>

namespace jaw
{
	struct Box;

	struct BoxWorld
	{
		BoxWorld();

		void add_box(Box* b);
		void remove_box(Box* b);
		void flush();
		void update(float dt);
		void clear();

		std::vector<Box*> to_add, to_remove;
		std::vector<Box*> boxes;
	};

	struct Box
	{
		Box();

		void update(float dt);

		void move(const vcm::vec2& d);

		BoxWorld* world;
		vcm::vec2 position;
		vcm::vec2 size;
	};
}