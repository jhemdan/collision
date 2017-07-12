#pragma once

namespace jaw
{
	struct Entity;
	struct Renderer;

	struct Graphic
	{
		Graphic();
		virtual ~Graphic();

		virtual void update(float dt);
		virtual void render(Renderer* renderer, Entity* entity);

		bool visible;
	};
}