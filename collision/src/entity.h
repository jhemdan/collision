#pragma once

#include "point.h"

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

		//checks for collision with other but at this position (pos)
		virtual bool check_collision(const Point& pos, Entity* other) const;
		virtual bool check_collision(const Point& pos) const;
		virtual void move(const vcm::vec2& d);

		void set_layer(int value);
		int get_layer() const { return _layer; }

		World* world;
		
		Point position;
		Point size; //for collision
		Point origin; //for collision
		Graphic* graphic;

		int _layer;

		vcm::vec2 _move_accum;

		bool solid;
	};
}