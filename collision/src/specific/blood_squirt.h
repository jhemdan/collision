#pragma once
#include "../entity.h"
#include "../particle_graphic.h"

namespace jaw
{
	struct BloodSquirt : Entity
	{
		ParticleGraphic particle_g;

		BloodSquirt(Texture2d* particle_tex);
		~BloodSquirt();

		bool alive;
		Point emit_pos;

		void update(float dt) override;
	};
}