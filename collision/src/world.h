#pragma once

#include <vector>
#include <vecmath/matrix.hpp>

namespace jaw
{
	struct Entity;
	struct Renderer;

	struct World
	{
		World();
		virtual ~World();

		virtual void add_entity(Entity* e);
		virtual void remove_entity(Entity* e);

		virtual void flush();
		virtual void update(float dt);
		virtual void render(Renderer* renderer);

		virtual void clear();

		std::vector<Entity*> _to_add, _to_remove;
		std::vector<Entity*> _buffer;
		std::vector<Entity*> entities;

		vcm::mat4 cam_tran;

		bool layer_flag;
	};
}