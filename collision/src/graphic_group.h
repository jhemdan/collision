#pragma once

#include "graphic.h"

#include <vector>

namespace jaw
{
	struct GraphicGroup : Graphic
	{
		GraphicGroup();

		void update(float dt) override;
		void render(Renderer* renderer, Entity* entity, const Point& offset) override;

		void add(Graphic* g);
		void remove(Graphic* g);

		std::vector<Graphic*> _list;
	};
}