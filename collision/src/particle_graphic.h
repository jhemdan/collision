#pragma once

#include "graphic.h"
#include "texture_2d.h"
#include "model.h"
#include "point.h"
#include "rect.h"

#include <vector>
#include <random>
#include <string>

namespace jaw
{
	struct Particle
	{
		Particle()
		{
			timer = 0.0f;
			start_life = 0.0f;
			life = 0.0f;
			frame = 0;
			color = vcm::vec4(1.0f);
			angle = 0.0f;
			scale = vcm::vec2(1.0f);
		}

		float timer;
		float start_life;
		float life;
		int frame; //absolute frame not relative to current anim
		vcm::vec2 position;
		vcm::vec4 color;
		float angle; //degrees
		vcm::vec2 scale;
		vcm::vec2 velocity;
	};

	struct ParticleMethod
	{
		ParticleMethod()
		{
			life_min = 0.5f;
			life_max = 1.0f;
			spawn_radius_min = 8.0f;
			spawn_radius_max = 16.0f;
			angle_min = 0.0f;
			angle_max = 360.0f;
			scale_min = { 0.5f, 0.5f };
			scale_max = { 1.0f, 1.0f };
			force_angle_min = 0.0f;
			force_angle_max = 360.0f;
			force_min = 50.0f;
			force_max = 100.0f;
			force_damping = 50.0f;
		}

		float life_min, life_max;
		float spawn_radius_min, spawn_radius_max;
		float angle_min, angle_max; //degrees
		vcm::vec2 scale_min, scale_max;
		float force_angle_min, force_angle_max; //degrees
		float force_min, force_max; //pixels per second
		float force_damping; //pixels per second
		std::string anim;
	};

	struct ParticleAnim
	{
		ParticleAnim()
		{
			//fps = 0;
			//loop = false;
			//life_mode = true;
		}

		ParticleAnim(const std::string& name, const std::vector<int>& frames)
		{
			this->name = name;
			this->frames = frames;
		}

		std::string name;
		std::vector<int> frames;
		//float fps;
		//bool loop;
		//bool life_mode; //if in life mode, the frames are changed by the particle's life/death time, otherwise regular fps is used
	};

	struct ParticleGraphic : Graphic
	{
		ParticleGraphic();

		void create(Texture2d* texture);
		void destroy();

		void update(float dt) override;
		void render(Renderer* renderer, Entity* entity, const Point& offset) override;

		//position is relative if relative is true
		//position is global if relative is false
		void emit(const Point& position);

		void _build_mesh();

		int get_anim(const std::string& name) const;
		void add_anim(const ParticleAnim& anim);

		Model model;
		Texture2d* texture;

		std::vector<Particle> particles;

		bool _dirty;
		float _timer;

		std::mt19937 mt_random;
		std::uniform_real_distribution<float> random_dist;

		ParticleMethod method;

		Point frame_size;

		std::vector<ParticleAnim> _anims;

		bool relative; //if the particles should be rendered relative to entity/graphic position
	};
}