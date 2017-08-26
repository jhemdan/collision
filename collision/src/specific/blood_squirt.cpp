#include "blood_squirt.h"
#include "../world.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	BloodSquirt::BloodSquirt(Texture2d* particle_tex)
	{
		alive = true;

		destroy_on_remove = true;

		particle_g.create(particle_tex);

		particle_g.frame_size = { 16, 16 };

		ParticleAnim panim = { "idle",{ 0, 1, 2, 3 } };
		particle_g.add_anim(panim);

		particle_g.relative = false;

		particle_g.method.anim = "idle";
		particle_g.method.force_angle_min = 90 - 10;
		particle_g.method.force_angle_max = 90 + 10;
		particle_g.method.spawn_radius_min = 4.0f;
		particle_g.method.spawn_radius_max = 8.0f;
		particle_g.method.force_min = 200.0f;
		particle_g.method.force_max = 250.0f;
		particle_g.method.gravity = 1000.0f;
		particle_g.method.life_min = 0.5f;
		particle_g.method.life_max = 0.75f;
		particle_g.method.scale_max = 1.5f;
		particle_g.method.scale_min = 1.0f;

		particle_g.method.angle_min = -20.0f;
		particle_g.method.angle_max = 20.0f;

		graphic = &particle_g;
	}

	BloodSquirt::~BloodSquirt()
	{
		particle_g.destroy();
	}

	void BloodSquirt::update(float dt)
	{
		Entity::update(dt);

		if (alive)
		{
			alive = false;
			for (int i = 0; i < 50; ++i)
			{
				particle_g.emit(emit_pos);
			}
		}
		else
		{
			if (particle_g.particles.empty())
			{
				world->remove_entity(this);
			}
		}
	}
}