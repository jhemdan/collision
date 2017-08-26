#include "flame_smoke.h"
#include "../world.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	FlameSmoke::FlameSmoke(Texture2d* particle_tex, const vcm::vec2& dir)
	{
		alive = true;

		destroy_on_remove = true;
	
		emit_timer = 0.0f;
		emit_per_second = 50.0f;

		particle_g.create(particle_tex);

		particle_g.frame_size = { 16, 16 };

		ParticleAnim panim = { "idle",{ 0, 1, 2, 3 } };
		particle_g.add_anim(panim);

		particle_g.relative = false;

		particle_g.method.anim = "idle";
		particle_g.method.force_angle_min = 90 - 20;
		particle_g.method.force_angle_max = 90 + 20;
		particle_g.method.spawn_radius_min = 4.0f;
		particle_g.method.spawn_radius_max = 8.0f;
		particle_g.method.force_min = 25.0f;
		particle_g.method.force_max = 40.0f;

		auto inv_dir = -dir;
		float inv_dir_angle = atan2(-inv_dir.y, inv_dir.x);
		if (inv_dir_angle < 0)
			inv_dir_angle += 2 * vcm::PI;

		inv_dir_angle *= vcm::DEG;

		particle_g.method.angle_min = -90 + inv_dir_angle - 20.0f;
		particle_g.method.angle_max = -90 + inv_dir_angle + 20.0f;

		graphic = &particle_g;
	}

	FlameSmoke::~FlameSmoke()
	{
		particle_g.destroy();
	}

	void FlameSmoke::update(float dt)
	{
		Entity::update(dt);

		if (alive)
		{
			emit_timer += dt;
			const float rate = 1.0f / emit_per_second;
			while (emit_timer >= rate)
			{
				emit_timer -= rate;
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