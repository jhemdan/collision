#include "sprite_graphic.h"
#include "renderer.h"
#include "entity.h"
#include "world.h"

namespace jaw
{
	SpriteGraphic::SpriteGraphic()
	{
		texture = nullptr;
	}

	void SpriteGraphic::create(Texture2d* tex)
	{
		this->texture = tex;

		this->model.mesh.vertices =
		{
			{ { 0.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { 0.0f, 1.0f },{ 0.0f, 1.0f } },
			{ { 1.0f, 1.0f },{ 1.0f, 1.0f } },
			{ { 1.0f, 0.0f },{ 1.0f, 0.0f } },
		};

		this->model.mesh.triangles =
		{
			{ 0, 1, 2 },
			{ 2, 3, 0 }
		};

		this->model.mbuffers.create(this->model.mesh);
	}

	void SpriteGraphic::destroy()
	{
		this->texture = nullptr;
		this->model.mesh = {};
		this->model.mbuffers.destroy();
	}

	void SpriteGraphic::render(Renderer* renderer, Entity* entity)
	{
		Graphic::render(renderer, entity);

		vcm::mat4 tran_mat = vcm::mat4
		{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ (float)entity->position.x, (float)entity->position.y, 0, 1 }
		};

		renderer->render(this, vcm::inverse(entity->world->cam_tran) * tran_mat);
	}
}