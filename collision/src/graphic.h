#pragma once

#include "point.h"

namespace jaw
{
	struct Entity;
	struct Renderer;

	struct Graphic
	{
		Graphic();
		virtual ~Graphic();

		virtual void update(float dt);
		virtual void render(Renderer* renderer, Entity* entity, const Point& offset);

		bool visible;
		Point position;
	};
}