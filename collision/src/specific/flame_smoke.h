#pragma once

#include "../entity.h"
#include "../particle_graphic.h"

namespace jaw
{
	struct FlameSmoke : Entity
	{
		ParticleGraphic particle_g;

		FlameSmoke(Texture2d* particle_tex, const vcm::vec2& dir);

		bool alive;
		Point emit_pos;

		float emit_timer;
		float emit_per_second;

		void update(float dt) override;
	};
}