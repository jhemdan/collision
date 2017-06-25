#include "particle_graphic.h"
#include "jaw_macros.h"
#include "entity.h"
#include "world.h"
#include "renderer.h"

#include <vecmath/matrix.hpp>
#include <vecmath/pi.hpp>

namespace jaw
{
	ParticleGraphic::ParticleGraphic()
		: texture(nullptr)
		, _dirty(false)
		, _timer(0.0f)

		, random_dist(0.0f, 1.0f)
	{
		std::random_device random_device;
		mt_random.seed(random_device());
	}

	void ParticleGraphic::create(Texture2d* texture)
	{
		JAW_ASSERT_MSG(texture, "No texture for ParticleGraphic::create()");

		this->texture = texture;

		this->model.mbuffers.create(this->model.mesh);
	}

	void ParticleGraphic::destroy()
	{
		model.mbuffers.destroy();

		*this = {};
	}

	void ParticleGraphic::update(float dt)
	{
		Graphic::update(dt);

		_timer += dt;

		const int eps = 10;
		
		if (_timer >= 1.0f / eps)
		{
			_timer = 0.0f;
			emit();
		}

		for (int i = 0; i < (int)particles.size(); )
		{
			_dirty = true;

			auto& p = particles[i];

			p.life -= dt;
			if (p.life <= 0.0f)
			{
				particles.erase(particles.begin() + i);
				continue;
			}

			p.timer += dt;
			
			p.angle += 100 * dt;

			++i;
		}

		if (_dirty)
		{
			_dirty = false;
			_build_mesh();
		}
	}
	
	void ParticleGraphic::render(Renderer* renderer, Entity* entity)
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

	void ParticleGraphic::emit()
	{
		Particle p;

		auto rnd = [this]() { return random_dist(mt_random); };
		auto rrange = [this, rnd](float min, float max) { return min + (max - min) * rnd(); };

		p.life = rrange(method.life_min, method.life_max);

		float pos_angle = rrange(0, 2 * vcm::PI);
		float pos_dist = rrange(method.spawn_radius_min, method.spawn_radius_max);
		p.position = { cos(pos_angle) * pos_dist, -sin(pos_angle) * pos_dist };
		
		p.angle = rrange(method.angle_min, method.angle_max);

		p.scale.x = rrange(method.scale_min.x, method.scale_max.x);
		p.scale.y = rrange(method.scale_min.y, method.scale_max.y);

		particles.push_back(p);

		_dirty = true;
	}

	void ParticleGraphic::_build_mesh()
	{
		auto& verts = model.mesh.vertices;
		auto& tris = model.mesh.triangles;

		verts.clear();
		tris.clear();

		//int tex_cols = texture->w / tile_w;

		for (const auto& p : particles)
		{
			vcm::mat2 rot_mat = vcm::from_angle(-p.angle * vcm::RAD);

			vcm::vec2 tex_size = { (float)texture->w, (float)texture->h };

			vcm::vec2 tl = rot_mat * (vcm::vec2(-1.0f, -1.0f) * (tex_size * 0.5f) * p.scale);
			vcm::vec2 bl = rot_mat * (vcm::vec2(-1.0f, 1.0f) * (tex_size * 0.5f) * p.scale);
			vcm::vec2 br = rot_mat * (vcm::vec2(1.0f, 1.0f) * (tex_size * 0.5f) * p.scale);
			vcm::vec2 tr = rot_mat * (vcm::vec2(1.0f, -1.0f) * (tex_size * 0.5f) * p.scale);

			vcm::vec2 top_left = p.position + tl;
			vcm::vec2 bottom_left = p.position + bl;
			vcm::vec2 bottom_right = p.position + br;
			vcm::vec2 top_right = p.position + tr;

			//int tex_x = p.frame % tex_cols * tile_w;
			//int tex_y = p.frame / tex_cols * tile_h;

			//float ws = 1.0f / texture->w;
			//float hs = 1.0f / texture->h;
			//vcm::vec2 tex_top_left = { (float)tex_x * ws, (float)tex_y * hs };
			vcm::vec2 tex_top_left = { 0, 0 };
			//vcm::vec2 tex_bottom_right = tex_top_left + vcm::vec2{ tile_w * ws, tile_h * hs };
			vcm::vec2 tex_bottom_right = { 1, 1 };

			unsigned vc = (unsigned)verts.size();

			verts.push_back({ top_left,{ tex_top_left.x, tex_top_left.y } });
			verts.push_back({ bottom_left,{ tex_top_left.x, tex_bottom_right.y } });
			verts.push_back({ bottom_right,{ tex_bottom_right.x, tex_bottom_right.y } });
			verts.push_back({ top_right,{ tex_bottom_right.x, tex_top_left.y } });

			tris.push_back(MeshTriangle{ 0, 1, 2 } +vc);
			tris.push_back(MeshTriangle{ 2, 3, 0 } +vc);
		}

		model.mbuffers.recreate(model.mesh);
	}
}