#pragma once

#include "graphic.h"
#include "texture_2d.h"
#include "model.h"
#include "rect.h"
#include "point.h"

#include <string>
#include <vector>
#include <vecmath/matrix.hpp>

namespace jaw
{
	struct SpriteAnim
	{
		SpriteAnim()
		{
			fps = 0;
			loop = false;
		}

		SpriteAnim(const std::string& name, const std::vector<int>& frames, int fps, bool loop)
		{
			this->name = name;
			this->frames = frames;
			this->fps = fps;
			this->loop = loop;
		}

		std::string name;
		std::vector<int> frames;
		int fps;
		bool loop;
	};

	struct SpriteGraphic : Graphic
	{
		SpriteGraphic();

		void create(Texture2d* tex);
		void destroy();

		void update(float dt) override;
		void render(Renderer* renderer, Entity* entity) override;

		void set_clip_rect(const Rect& r);
		Rect get_clip_rect() const { return _clip_rect; }

		void _calc_clip_rect();

		int get_anim(const std::string& name) const;
		void add_anim(const SpriteAnim& anim);
		void play_anim(const std::string& name);
		void _next_frame();

		Model model;
		Texture2d* texture;

		bool _dirty_clip;
		Rect _clip_rect;

		std::vector<SpriteAnim> _anims;
		float _anim_timer;		
		int _cur_anim;
		int _dest_anim;
		int _cur_frame;
		bool _playing_anim;

		Point frame_size; //for clip_rect calc during animation
	};
}