#include "entity.h"
#include "graphic.h"
#include "world.h"

namespace jaw
{
	Entity::Entity()
	{
		world = nullptr;
		size = { 1, 1 };
		graphic = nullptr;

		_layer = 0;

		solid = false;

		parent = nullptr;

		destroy_on_remove = false;
	}

	Entity::~Entity()
	{

	}

	void Entity::on_added()
	{

	}

	void Entity::on_removed()
	{

	}

	void Entity::update(float dt)
	{
		if (parent)
		{
			position = parent->position + rel_position;
		}

		if (graphic)
			graphic->update(dt);
	}

	void Entity::render(Renderer* renderer)
	{
		if (graphic)
			graphic->render(renderer, this, Point());
	}

	bool Entity::check_intersection(const Point& pos, Entity* other) const
	{
		if (pos.x + origin.x >= other->position.x + other->origin.x + other->size.x)
		{
			return false;
		}

		if (pos.y + origin.y >= other->position.y + other->origin.y + other->size.y)
		{
			return false;
		}

		if (pos.x + origin.x + size.x <= other->position.x + other->origin.x)
		{
			return false;
		}

		if (pos.y + origin.y + size.y <= other->position.y + other->origin.y)
		{
			return false;
		}

		return true;
	}

	bool Entity::check_intersection(const Point& pos, const std::string& type, std::vector<Entity*>& out_results) const
	{
		if (!world)
			return false;

		bool ret = false;

		for (auto other : world->entities)
		{
			if (other == this)
				continue;

			if (other->type == type)
			{
				if (check_intersection(pos, other))
				{
					ret = true;
					out_results.push_back(other);
				}
			}
		}

		return ret;
	}

	bool Entity::check_collision(const Point& pos, Entity* other) const
	{
		if (solid && other->solid)
		{
			if (check_intersection(pos, other))
				return true;
		}

		return false;
	}

	bool Entity::check_collision(const Point& pos) const
	{
		if (!world || !solid)
			return false;

		for (auto other : world->entities)
		{
			if (other == this)
				continue;

			if (check_collision(pos, other))
				return true;
		}

		return false;
	}

	void Entity::move(const vcm::vec2& d_)
	{
		_move_accum += d_;
		Point d((int)std::round(_move_accum.x), (int)std::round(_move_accum.y));
		_move_accum -= vcm::vec2{ (float)d.x, (float)d.y };

		if (check_collision(position))
		{
			position += d;
			return;
		}

		int sign = 0;
		if (d.x != 0)
		{
			if (check_collision(Point(position.x + d.x, position.y)))
			{
				sign = d.x > 0 ? 1 : -1;
				while (d.x != 0)
				{
					if (check_collision(Point(position.x + sign, position.y)))
						break;

					position.x += sign;
					d.x -= sign;
				}
			}
			else
			{
				position.x += d.x;
			}
		}

		if (d.y != 0)
		{
			if (check_collision(Point(position.x, position.y + d.y)))
			{
				sign = d.y > 0 ? 1 : -1;
				while (d.y != 0)
				{
					if (check_collision(Point(position.x, position.y + sign)))
						break;

					position.y += sign;
					d.y -= sign;
				}
			}
			else
			{
				position.y += d.y;
			}
		}
	}

	void Entity::set_layer(int value)
	{
		if (_layer != value)
		{
			_layer = value;

			if(world)
				world->layer_flag = true;
		}
	}
}