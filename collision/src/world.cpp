#include "world.h"
#include "entity.h"
#include "jaw_macros.h"
#include "cam_ent.h"

#include <algorithm>

namespace jaw
{
	World::World()
	{
		layer_flag = false;

		cam_ent = new CamEnt();

		cam_ent->size = { 400.0f, 300.0f };

		add_entity(cam_ent);
	}

	World::~World()
	{
		delete cam_ent;
	}

	void World::add_entity(Entity* e)
	{
		if(e)
			_to_add.push_back(e);
	}

	void World::remove_entity(Entity* e)
	{
		if(e)
			_to_remove.push_back(e);
	}

	void World::flush()
	{
		//go through pending added entities and add them to main list
		//calling on_added() and giving them this world
		if (!_to_add.empty())
		{
			for (int i = 0; i < (int)_to_add.size(); ++i)
			{
				auto e = _to_add[i];

				//make sure the world for this entity is null or this world
				//the reason I allow "this" to be the world of the entity is so if we add it multiple times it doesn't matter
				JAW_ASSERT_MSG(e->world == nullptr || e->world == this, "Bad entity world value for World::flush()");
				if (!e->world)
				{
					entities.push_back(e);
					e->world = this;
					
					//need to sort by layers everytime a new entity is added
					layer_flag = true;

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

		//go through pending removed entities and remove them from the main list
		//calling on_removed() and changing their world to nullptr
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
				//give them back this world real quick so they can work with it in on_removed()
				e->world = this;
				e->on_removed();
				
				//take away the world again
				e->world = nullptr;

				if (e->destroy_on_remove)
					delete e;
			}
			_buffer.clear();
		}
	}

	void World::update(float dt)
	{
		flush();

		//update all entities
		for (auto e : entities)
		{
			e->update(dt);
		}

		//sort the entities by their layer number
		if (layer_flag)
		{
			auto sorter = [](Entity* a, Entity* b)
			{
				return a->_layer < b->_layer;
			};

			std::sort(entities.begin(), entities.end(), sorter);

			layer_flag = false;
		}

		cam_tran = cam_ent->get_cam_tran();
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
		//call on_removed() on all entities
		for (auto e : entities)
		{
			e->on_removed();

			if (e->destroy_on_remove)
				delete e;
		}

		_to_add.clear();
		_to_remove.clear();

		entities.clear();
	}
}