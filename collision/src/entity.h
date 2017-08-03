#pragma once

#include "point.h"

#include <string>
#include <vector>

namespace jaw
{
	struct World;
	struct Graphic;
	struct Renderer;

	struct Entity
	{
		Entity();
		virtual ~Entity();

		virtual void on_added();
		virtual void on_removed();

		virtual void update(float dt);
		virtual void render(Renderer* renderer);

		//checks for intersection with other but at this position (pos) even if not solid
		virtual bool check_intersection(const Point& pos, Entity* other) const;

		//checks for intersection with others of this type
		virtual bool check_intersection(const Point& pos, const std::string& type, std::vector<Entity*>& out_results) const;

		//checks for intersection only if both are solid
		virtual bool check_collision(const Point& pos, Entity* other) const;
		//checks for intersection with other solid entities
		virtual bool check_collision(const Point& pos) const;

		virtual void move(const vcm::vec2& d);

		void set_layer(int value);
		int get_layer() const { return _layer; }

		Point get_center_pos() const { return position + origin + size * 0.5f; }

		World* world;
		
		Point position;
		Point size; //for collision
		Point origin; //for collision
		Graphic* graphic;

		int _layer;

		vcm::vec2 _move_accum;

		bool solid;

		Entity* parent;
		Point rel_position; //position relative to parent

		std::string type;
	};
}