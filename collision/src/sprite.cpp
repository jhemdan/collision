#include "sprite.h"

namespace jaw
{
	Sprite::Sprite()
	{
		texture = nullptr;
	}

	void Sprite::create(Texture2d* tex)
	{
		this->texture = tex;

		this->model.mesh.vertices =
		{
			{ { 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { 0.0f, 1.0f }, { 0.0f, 1.0f } },
			{ { 1.0f, 1.0f }, { 1.0f, 1.0f } },
			{ { 1.0f, 0.0f }, { 1.0f, 0.0f } },
		};

		this->model.mesh.triangles =
		{
			{ 0, 1, 2 },
			{ 2, 3, 0 }
		};

		this->model.mbuffers.create(this->model.mesh);
	}

	void Sprite::destroy()
	{
		this->texture = nullptr;
		this->model.mesh = {};
		this->model.mbuffers.destroy();
	}
}