#include "graphic_group.h"

namespace jaw
{
	GraphicGroup::GraphicGroup()
	{

	}

	void GraphicGroup::update(float dt)
	{
		Graphic::update(dt);

		for (auto g : _list)
		{
			g->update(dt);
		}
	}

	void GraphicGroup::render(Renderer* renderer, Entity* entity, const Point& offset)
	{
		Graphic::render(renderer, entity, offset);

		for (auto g : _list)
		{
			g->render(renderer, entity, offset + position);
		}
	}

	void GraphicGroup::add(Graphic* g)
	{
		auto it = std::find(_list.begin(), _list.end(), g);
		if (it == _list.end())
		{
			_list.push_back(g);
		}
	}

	void GraphicGroup::remove(Graphic* g)
	{
		auto it = std::find(_list.begin(), _list.end(), g);
		if (it != _list.end())
		{
			_list.erase(it);
		}
	}
}