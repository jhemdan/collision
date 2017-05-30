#include "kick_gen.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	KickWave::KickWave()
		: length(0.0f)
	{

	}

	void KickWave::generate(float len)
	{
		this->length = len;

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

	float KickWave::get_value(float t) const
	{
		float start_freq = 66.66666667f;
		float end_freq = 60;

		float start_amp = 0.9f;
		float end_amp = 0.05f;

		float freq = start_freq + (end_freq - start_freq) * (t / length);
		float ampl = start_amp + (end_amp - start_amp) * (t / length);

		float x = (t * freq) * (2 * vcm::PI);

		float value_sin = sin(x);
		float value_tri = triangle_wave(x);
		float value_total = mix_wave(value_sin, value_tri, 0.5f);

		float value = value_total;

		value *= ampl;

		//float random = ((rand() % 1000) - 500) / 500.0f * 0.05f;
		float random = _get_random_value(t) * 0.35f;
		value += (abs(value) / 1.0f) * random;

		float volume = 0.7f;
		value *= volume;

		return value;
	}

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
			for (int y = 0; y < out_bmp.h; ++y)
			{
				out_bmp.set_pixel(x, y, 0xaaffffff);
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