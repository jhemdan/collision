#include "world.h"
#include "entity.h"
#include "jaw_macros.h"

#include <algorithm>

namespace jaw
{
	World::World()
	{
		layer_flag = false;
	}

	World::~World()
	{

	}

	void World::add_entity(Entity* e)
	{
		_to_add.push_back(e);
	}

	void World::remove_entity(Entity* e)
	{
		_to_remove.push_back(e);
	}

	void World::flush()
	{
		if (!_to_add.empty())
		{
			for (int i = 0; i < (int)_to_add.size(); ++i)
			{
				auto e = _to_add[i];

				JAW_ASSERT_MSG(e->world == nullptr || e->world == this, "Bad entity world value for World::flush()");
				if (!e->world)
				{
					entities.push_back(e);
					e->world = this;

					_buffer.push_back(e);
				}
			}
			_to_add.clear();

			for (auto e : _buffer)
			{
				e->on_added();
			}
			_buffer.clear();
		}

		if (!_to_remove.empty())
		{
			for (int i = 0; i < (int)_to_remove.size(); ++i)
			{
				auto e = _to_remove[i];

				if (e->world == this)
				{
					auto it = std::find(entities.begin(), entities.end(), e);
					if (it != entities.end())
					{
						entities.erase(it);
						e->world = nullptr;

						_buffer.push_back(e);
					}
				}
			}
			_to_remove.clear();

			for (auto e : _buffer)
			{
				e->on_removed();
			}
			_buffer.clear();
		}
	}

	void World::update(float dt)
	{
		flush();

		for (auto e : entities)
		{
			e->update(dt);
		}

		if (layer_flag)
		{
			auto sorter = [](Entity* a, Entity* b)
			{
				return a->_layer < b->_layer;
			};

			std::sort(entities.begin(), entities.end(), sorter);

			layer_flag = false;
		}
	}

	void World::render(Renderer* renderer)
	{
		for (auto e : entities)
		{
			e->render(renderer);
		}
	}

	void World::clear()
	{
		_to_add.clear();
		_to_remove.clear();
		entities.clear();
	}
}