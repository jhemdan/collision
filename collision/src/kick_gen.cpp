#include "kick_gen.h"
#include "log.h"

#include <vecmath/pi.hpp>
#include <algorithm>

namespace jaw
{
	//float KickWave::FreqBox::no_box_sin_offset = 0.0f;

	KickWave::Clip::Clip()
		: position(0.0f)
		, length(0.0f)
		, freq(0.0f)
	{

	}

	KickWave::MixedClip::MixedClip()
		: clip_a(nullptr)
		, clip_b(nullptr)
	{

	}

	KickWave::KickWave()
		: length(0.0f)
		/*
		, no_box_freq(0.0f)
		, no_box_sin_offset(0.0f)
		*/
	{

	}

	KickWave::~KickWave()
	{
		for (auto clip : clips)
			delete clip;
	}

	void KickWave::generate(float len)
	{
		this->length = len;

		clips.resize(3);

		clips[0] = new Clip();
		clips[0]->freq = 111.111111111f;
		clips[0]->position = 0.0f;
		clips[0]->length = 0.0275f;

		clips[1] = new Clip();
		clips[1]->freq = 66.6666666666f;
		clips[1]->position = clips[0]->position + clips[0]->length - 0.01f;
		clips[1]->length = 0.1075f;

		clips[2] = new Clip();
		clips[2]->freq = 51.28f;
		clips[2]->position = clips[1]->position + clips[1]->length - 0.01f;
		clips[2]->length = 0.115f;

		auto clip_sorter = [](Clip* a, Clip* b)
		{
			return a->position < b->position;
		};

		std::sort(clips.begin(), clips.end(), clip_sorter);

		MixedClip* prev_mixed_clip = nullptr;

		for (int i = 0; i < (int)clips.size(); ++i)
		{
			if (i == 0)
				continue;

			MixedClip* mixed_clip = new MixedClip();

			if (prev_mixed_clip)
			{
				mixed_clip->clip_a = prev_mixed_clip;
			}
			else
			{
				mixed_clip->clip_a = clips[0];
			}

			mixed_clip->clip_b = clips[i];

			mixed_clips.push_back(mixed_clip);

			prev_mixed_clip = mixed_clip;
		}

		/*
		int rand_table_size = 50;
		
		random_table.resize(rand_table_size);
		for (int i = 0; i < rand_table_size; ++i)
			random_table[i] = ((rand() % 1000) - 500) / 500.0f;

		rand_ranges.resize(rand_table_size);

		int num_secs = rand_table_size - 1;
		float sec_size = 1.0f / num_secs;
		for (int i = 0; i < rand_table_size; ++i)
		{
			float j = i * sec_size;
			rand_ranges[i].min = j - (sec_size * 0.5f);
			rand_ranges[i].max = j + (sec_size * 0.5f);

			if (i == 0)
			{
				rand_ranges[i].min = 0.0f;
			}
			if (i == rand_table_size - 1)
			{
				rand_ranges[i].max = 1.0f;
			}
		}

		no_box_sin_offset = sin_offsets[2];
		FreqBox::no_box_sin_offset = no_box_sin_offset;

		no_box_freq = 51.28f;
		const float start_lerp_offset = 0.005f;

		float ta = 0.0f;
		float tb = 0.0275f;
		float tc = 0.0275f + 0.1075f;

		FreqBox freq_box;

		freq_box.no_box_freq = no_box_freq;
		
		freq_box.min = ta;
		freq_box.max = tb;
		freq_box.freq = 111.111111f;
		freq_box.start_lerp = freq_box.max - start_lerp_offset;
		freq_box.sin_offset = sin_offsets[0];

		freq_boxes.push_back(freq_box);

		freq_box.min = tb;
		freq_box.max = tc;
		freq_box.freq = 66.6666666f;
		freq_box.start_lerp = freq_box.max - start_lerp_offset;
		freq_box.sin_offset = sin_offsets[1];

		freq_boxes.push_back(freq_box);

		freq_boxes[0].next = &freq_boxes[1];
		freq_boxes[1].next = nullptr;
		*/
	}

	float square_wave(float t)
	{
		float val = sin(t);
		int sign = val < 0 ? -1 : (val > 0 ? 1 : 0);
		return (float)sign;
	}

	float triangle_wave(float t)
	{
		float val = asin(sin(t)) * (1.0f / (vcm::PI / 2.0f));
		return val;
	}

	float mix_wave(float val_a, float val_b, float amount)
	{
		return val_a + (val_b - val_a) * amount;
	}

	/*
	float KickWave::_get_freq(float t) const
	{
		for (const auto& box : freq_boxes)
		{
			if (t >= box.min && t <= box.max)
			{
				return box.get_freq(t);
			}
		}

		return no_box_freq;
	}

	const KickWave::FreqBox* KickWave::_get_freq_box(float t) const
	{
		for (const auto& box : freq_boxes)
		{
			if (t >= box.min && t <= box.max)
			{
				return &box;
			}
		}

		return nullptr;
	}
	*/

	float KickWave::Clip::get_value(float t) const
	{
		if (t >= get_min() && t <= get_max())
		{
			float my_t = t - position;
			float x = (my_t * freq) * (2 * vcm::PI);

			return sin(x);
		}

		return 0.0f;
	}

	float KickWave::MixedClip::get_value(float t) const
	{
		float value_a = clip_a->get_value(t);
		float value_b = clip_b->get_value(t);

		//clip b is past the end of clip a
		if (clip_b->get_min() > clip_a->get_max())
		{
			if (t <= clip_a->get_max())
				return value_a;
			if (t >= clip_b->get_min())
				return value_b;

			return 0.0f;
		}
		else if(clip_b->get_min() < clip_a->get_max()) //clip b collides with clip a
		{
			//clip b is fully inside clip a
			if (clip_b->get_max() < clip_a->get_max())
			{
				float center_clip_b = clip_b->get_center();
				float lerp_t;

				if (t < center_clip_b)
				{
					lerp_t = (t - clip_b->get_min()) / (center_clip_b - clip_b->get_min());

					if (lerp_t < 0)
						lerp_t = 0;
					if (lerp_t > 1)
						lerp_t = 1;

					return mix_wave(value_a, value_b, lerp_t);
				}
				else if (t == center_clip_b)
				{
					return value_b;
				}
				else
				{
					lerp_t = 1.0f - (t - clip_b->get_min()) / (center_clip_b - clip_b->get_min());

					if (lerp_t < 0)
						lerp_t = 0;
					if (lerp_t > 1)
						lerp_t = 1;

					return mix_wave(value_a, value_b, lerp_t);
				}
			}
			else //clip b is partially inside clip a
			{
				float lerp_t = (t - clip_b->get_min()) / (clip_a->get_max() - clip_b->get_min());

				if (lerp_t < 0)
					lerp_t = 0;
				if (lerp_t > 1)
					lerp_t = 1;

				return mix_wave(value_a, value_b, lerp_t);
			}
		}
		else //clip b is immidiately after clip a
		{
			if (t <= clip_a->get_max())
				return value_a;

			return value_b;
		}
	}

	float KickWave::MixedClip::get_min() const
	{
		return clip_a->get_min();
	}

	float KickWave::MixedClip::get_max() const
	{
		return clip_b->get_max();
	}

	float KickWave::get_value(float t) const
	{
		/*
		float start_freq = 66.66666667f;
		float end_freq = 60;

		float start_amp = 0.9f;
		float end_amp = 0.05f;

		float freq = start_freq + (end_freq - start_freq) * (t / length);
		float ampl = start_amp + (end_amp - start_amp) * (t / length);

		ampl = 1.0f;
		freq = _get_freq(t);

		float x = (t * freq) * (2 * vcm::PI);

		float value_sin = sin(x);
		float value_tri = triangle_wave(x);
		float value_total = mix_wave(value_sin, value_tri, 0.5f);

		float value = value_total;

		value *= ampl;
		*/

		float value = 0.0f;

		if (!mixed_clips.empty())
		{
			auto root = mixed_clips[mixed_clips.size() - 1];
			value = root->get_value(t);
		}
		else
		{
			if (!clips.empty())
			{
				value = clips[0]->get_value(t);
			}
		}

		/*
		auto freq_box = _get_freq_box(t);
		if (freq_box)
		{
			value = freq_box->get_sin(t);
		}
		else
		{
			float x = FreqBox::get_boxless_x(t, no_box_freq);
			value = sin(x);
		}
		*/

		//float random = ((rand() % 1000) - 500) / 500.0f * 0.05f;
		//float random = _get_random_value(t) * 0.35f;
		//value += (abs(value) / 1.0f) * random;

		float volume = 0.7f;
		value *= volume;

		return value;
	}

	/*
	float KickWave::_get_random_value(float t) const
	{
		float x = t / length; //t but normalized to >= 0.0f and < 1.0f
		int rand_table_size = (int)random_table.size();

		int curi, lefti, righti;
		for (int i = 0; i < rand_table_size; ++i)
		{
			if (x >= rand_ranges[i].min && x <= rand_ranges[i].max)
			{
				curi = i;
				break;
			}
		}

		if (x < rand_ranges[curi].get_center())
		{
			lefti = curi - 1;
			righti = curi;

			//should never happen
			if (lefti < 0)
				lefti = 0;
		}
		else if (x > rand_ranges[curi].get_center())
		{
			lefti = curi;
			righti = curi + 1;

			//should never happen
			if (righti > rand_table_size - 1)
				righti = rand_table_size - 1;
		}
		else
		{
			lefti = righti = curi;
		}

		if (lefti == righti)
		{
			float value = random_table[curi];
			return value;
		}

		float x2 = (x - rand_ranges[lefti].get_center()) / (rand_ranges[righti].get_center() - rand_ranges[lefti].get_center());
		float valuel = random_table[lefti];
		float valuer = random_table[righti];
		float value = valuel + (valuer - valuel) * x2;
		return value;
	}
	*/

	void KickWave::gen_bmp(Bitmap& out_bmp) const
	{
		out_bmp = {};

		out_bmp.h = 100;
		out_bmp.w = 400;
		out_bmp.format = BITMAP_RGBA;
		out_bmp.calc_pitch();
		out_bmp.data.resize(out_bmp.pitch * out_bmp.h);

		for (int x = 0; x < out_bmp.w; ++x)
		{
			unsigned color = 0xaaffffff;

			/*
			float t = (x / (float)out_bmp.w) * length;
			for (const auto& box : freq_boxes)
			{
				if (t >= box.start_lerp && t <= box.max)
					color = 0xaaffaaff;
			}
			*/

			for (int y = 0; y < out_bmp.h; ++y)
			{
				out_bmp.set_pixel(x, y, color);
			}
		}

		//DONT DELETE
		auto mix_color = [](int r, int g, int b, int r2, int g2, int b2, float t)
		{
			int r_ = r + (r2 - r) * t;
			int g_ = g + (g2 - g) * t;
			int b_ = b + (b2 - b) * t;

			r %= 256;
			g %= 256;
			b %= 256;

			unsigned color = (0xff << 24) | (b << 16) | (g << 8) | r;
			return color;
		};

		int pixels_per_second = 400 / length;
		for (auto clip : clips)
		{
			int x = clip->get_min() * pixels_per_second;
			int dest_x = clip->get_max() * pixels_per_second;
			for (int y = 0; y < out_bmp.h; ++y)
			{
				for (int x = 0; x < dest_x; ++x)
				{
					out_bmp.set_pixel(x, y, 0xff00ffff);
				}
			}
		}
		
		bool has_prev_val = false;
		int prev_val;
		for (int x = 0; x < 400; ++x)
		{
			float x_ = x / 400.0f; //0.0f, 1.0f

			float value = get_value(x_ * length);
			int valuei = (int)round(value * 50) + 50;

			if (valuei >= 100)
				valuei = 99;

			if (has_prev_val)
			{
				out_bmp.draw_line(x - 1, prev_val, x, valuei, 0xff0000ff);
				out_bmp.set_pixel(x - 1, prev_val, 0xffff00ff);
			}

			out_bmp.set_pixel(x, valuei, 0xffff00ff);

			prev_val = valuei;
			has_prev_val = true;
		}
	}
}