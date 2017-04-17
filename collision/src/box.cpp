#include "box.h"

namespace jaw
{
	BoxWorld::BoxWorld()
	{

	}

	void BoxWorld::add_box(Box* b)
	{
		to_add.push_back(b);
	}

	void BoxWorld::remove_box(Box* b)
	{
		to_remove.push_back(b);
	}

	void BoxWorld::flush()
	{
		if (!to_add.empty())
		{
			//use i instead of : because box->on_added might add a box to this list while we are iterating
			for (int i = 0; i < (int)to_add.size(); ++i)
			{
				auto b = to_add[i];

				if (b->world != this)
				{
					boxes.push_back(b);
					b->world = this;
				}
			}

			to_add.clear();
		}

		if (!to_remove.empty())
		{
			for (int i = 0; i < (int)to_remove.size(); ++i)
			{
				auto b = to_remove[i];

				if (b->world == this)
				{
					auto it = std::find(boxes.begin(), boxes.end(), b);
					if (it != boxes.end())
					{
						boxes.erase(it);
						b->world = nullptr;
					}
				}
			}

			to_remove.clear();
		}
	}

	void BoxWorld::update(float dt)
	{
		flush();

		for (auto* b : boxes)
		{
			b->update(dt);
		}
	}

	void BoxWorld::clear()
	{
		//dont call on_removed

		to_add.clear();
		to_remove.clear();
		boxes.clear();
	}

	Box::Box()
		: world(nullptr)
		, size(32, 32)
	{

	}

	void Box::update(float dt)
	{

	}

	void Box::move(const vcm::vec2& d)
	{

	}
}