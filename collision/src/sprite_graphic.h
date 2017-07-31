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

		SpriteAnim(const std::string& name, const std::vector<int>& frames, float fps, bool loop)
		{
			this->name = name;
			this->frames = frames;
			this->fps = fps;
			this->loop = loop;
		}

		std::string name;
		std::vector<int> frames;
		float fps;
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
		void _build_uvs();

		int get_anim(const std::string& name) const;

		int get_cur_anim_index() const { return _cur_anim; }
		const SpriteAnim& get_anim(int index) const { return _anims[index]; }
		int get_cur_anim_frame() const { return _cur_frame; }

		float get_anim_timer() const { return _anim_timer; }
		void set_anim_timer(float t) { _anim_timer = t; }

		void add_anim(const SpriteAnim& anim);
		void play_anim(const std::string& name);
		//sets the frame of the currently playing anim
		void set_anim_frame(int f);
		void stop_anim(); //resets the anim
		void _next_frame();

		//returns true if the current animation has reached its end
		bool has_reached_end() const { return _reached_end; }
		bool is_playing_anim() const { return _playing_anim; }

		Model model;
		Texture2d* texture;

		bool _dirty_clip;
		Rect _clip_rect;

		std::vector<SpriteAnim> _anims;
		float _anim_timer;		
		int _cur_anim;
		int _cur_frame;
		bool _playing_anim;

		Point frame_size; //for clip_rect calc during animation

		Point origin;

		bool _reached_end;

		vcm::vec2 scale;

		vcm::vec4 color;
	};
}