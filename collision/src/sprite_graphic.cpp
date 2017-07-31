#include "sprite_graphic.h"
#include "renderer.h"
#include "entity.h"
#include "world.h"
#include "exception.h"
#include "jaw_macros.h"

namespace jaw
{
	SpriteGraphic::SpriteGraphic()
	{
		texture = nullptr;

		_dirty_clip = false;

		_anim_timer = 0.0f;
		_cur_anim = -1;
		_cur_frame = 0;
		_playing_anim = false;

		_reached_end = false;

		scale = vcm::vec2{ 1.0f };

		color = vcm::vec4{ 1.0f };
	}

	void SpriteGraphic::create(Texture2d* tex)
	{
		JAW_ASSERT_MSG(tex, "No texture for SpriteGraphic::create()");

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

		set_clip_rect(Rect(tex->w, tex->h));

		frame_size = Point(tex->w, tex->h);
	}

	void SpriteGraphic::destroy()
	{
		this->model.mbuffers.destroy();

		*this = {};
	}

	void SpriteGraphic::update(float dt)
	{
		Graphic::update(dt);

		if (_playing_anim)
		{
			_anim_timer += dt;

			if (_anim_timer >= 1.0f / _anims[_cur_anim].fps)
			{
				_next_frame();
			}
		}

		_calc_clip_rect();
	}

	void SpriteGraphic::render(Renderer* renderer, Entity* entity)
	{
		Graphic::render(renderer, entity);

		if (_dirty_clip)
		{
			_dirty_clip = false;
			_build_uvs();
		}

		vcm::mat4 tran_mat = vcm::mat4
		{
			{ scale.x, 0, 0, 0 },
			{ 0, scale.y, 0, 0 },
			{ 0, 0, 1, 0 },
			{ (float)entity->position.x - (float)origin.x, (float)entity->position.y - (float)origin.y, 0, 1 }
		};

		renderer->render(this, vcm::inverse(entity->world->cam_tran) * tran_mat);
	}

	void SpriteGraphic::set_clip_rect(const Rect& r)
	{
		if (_clip_rect != r)
		{
			_dirty_clip = true;
			_clip_rect = r;
		}
	}

	void SpriteGraphic::_calc_clip_rect()
	{
		if (_cur_anim != -1)
		{
			int actual_frame = _anims[_cur_anim].frames[_cur_frame];

			int cols = texture->w / frame_size.x;

			Rect cr;
			cr.x = actual_frame % cols * frame_size.x;
			cr.y = actual_frame / cols * frame_size.y;
			cr.w = frame_size.x;
			cr.h = frame_size.y;
			set_clip_rect(cr);
		}
		else
		{
			Rect cr(texture->w, texture->h);
			set_clip_rect(cr);
		}
	}

	void SpriteGraphic::_build_uvs()
	{
		vcm::vec4 uv_rect;
		uv_rect.x = (float)_clip_rect.x / texture->w;
		uv_rect.y = (float)_clip_rect.y / texture->h;
		uv_rect.z = (float)_clip_rect.w / texture->w;
		uv_rect.w = (float)_clip_rect.h / texture->h;

		auto verts = this->model.mesh.vertices.data();
		verts[0].tex_coords = { uv_rect.x, uv_rect.y };
		verts[1].tex_coords = { uv_rect.x, uv_rect.y + uv_rect.w };
		verts[2].tex_coords = { uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w };
		verts[3].tex_coords = { uv_rect.x + uv_rect.z, uv_rect.y };

		this->model.mbuffers.recreate(this->model.mesh);
	}

	int SpriteGraphic::get_anim(const std::string& name) const
	{
		for (int i = 0; i < (int)_anims.size(); ++i)
		{
			if (_anims[i].name == name)
				return i;
		}

		return -1;
	}

	void SpriteGraphic::add_anim(const SpriteAnim& anim)
	{
		JAW_ASSERT_MSG(anim.frames.size(), "No frames for SpriteGraphic::add_anim()");

		if (get_anim(anim.name) == -1)
		{
			_anims.push_back(anim);
		}
	}

	void SpriteGraphic::play_anim(const std::string& name)
	{
		int dest_anim = get_anim(name);

		if (_cur_anim != dest_anim)
		{
			_cur_anim = dest_anim;
			_cur_frame = 0;
			_anim_timer = 0.0f;
			_reached_end = false;

			_calc_clip_rect();

			if (dest_anim != -1)
			{
				_playing_anim = true;
			}
		}
	}

	void SpriteGraphic::set_anim_frame(int f)
	{
		if (_cur_anim != -1 && _playing_anim)
		{
			_cur_frame = f;
			_anim_timer = 0.0f;

			if (_cur_frame < 0)
				_cur_frame = 0;

			if (_cur_frame >= (int)_anims[_cur_anim].frames.size())
			{
				if (_anims[_cur_anim].loop)
				{
					_cur_frame = 0;
				}
				else
				{
					_cur_frame = (int)_anims[_cur_anim].frames.size() - 1;
					_reached_end = true;
				}
			}
		}
	}

	void SpriteGraphic::stop_anim()
	{
		_cur_anim = -1;
		_playing_anim = false;
		_reached_end = false;
		_cur_frame = 0;
		_anim_timer = 0.0f;

		set_clip_rect(Rect(texture->w, texture->h));
	}

	void SpriteGraphic::_next_frame()
	{
		set_anim_frame(_cur_frame + 1);
	}
}