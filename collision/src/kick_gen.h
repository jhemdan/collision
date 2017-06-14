#pragma once

#include "bitmap.h"

#include <vector>
#include <vecmath/pi.hpp>

namespace jaw
{
	float square_wave(float t);
	float triangle_wave(float t);
	float mix_wave(float val_a, float val_b, float amount);

	struct KickWave
	{
		KickWave();
		~KickWave();

		void generate(float len);

		/*
		float _get_freq(float t) const;
		*/

		float get_value(float t) const;

		void gen_bmp(Bitmap& out_bmp) const;

		float length;

		struct Clip
		{
			Clip();

			float position;
			float length;
			float freq;

			float amp_a, amp_b, amp_c;

			virtual float get_value(float t) const;

			virtual float get_min() const { return position; }
			virtual float get_max() const { return position + length; }

			float get_center() const
			{
				float min = get_min();
				float max = get_max();

				return min + (max - min) * 0.5f;
			}

			/*
			float start_lerp;

			FreqBox* next; //the original array shouldn't change after this is set
			float no_box_freq;

			float sin_offset;
			static float no_box_sin_offset;

			//t = actual time
			float get_freq(float t) const
			{
			if (t >= start_lerp)
			{
			float lerp_t = (t - start_lerp) / (max - start_lerp);

			float next_freq = next ? next->freq : no_box_freq;
			float lerped_freq = freq + (next_freq - freq) * lerp_t;
			return lerped_freq;
			}

			return freq;
			}

			float get_lerp_t(float t) const
			{
			if (t >= start_lerp)
			{
			return (t - start_lerp) / (max - start_lerp);
			}

			return 0.0f;
			}

			float get_x(float t) const
			{
			return (t * freq) * (2 * vcm::PI) + sin_offset;
			}

			static float get_boxless_x(float t, float no_box_freq)
			{
			return (t * no_box_freq) * (2 * vcm::PI) + no_box_sin_offset;
			}

			float get_sin(float t) const
			{
			float xa = get_x(t);
			float xb;
			if (next)
			xb = next->get_x(t);
			else
			xb = get_boxless_x(t, no_box_freq);

			return mix_wave(sin(xa), sin(xb), get_lerp_t(t));
			}
			};

			std::vector<FreqBox> freq_boxes;
			float no_box_freq;
			float no_box_sin_offset;

			const FreqBox* _get_freq_box(float t) const;
			*/
		};

		std::vector<Clip*> clips;

		struct MixedClip : Clip
		{
			MixedClip();

			Clip* clip_a; //if mixed_clip only has clip_a then this tree is just one clip
			Clip* clip_b;

			float get_value(float t) const override;
			float get_min() const override;
			float get_max() const override;
		};

		std::vector<MixedClip*> mixed_clips; //pool

		/*
		float _get_random_value(float t) const;

		float length;
		std::vector<float> random_table; //-1.0f, 1.0f

		struct ValueRange
		{
			float min;
			float max;

			float get_center() const
			{
				return min + (max - min) * 0.5f;
			}
		};

		std::vector<ValueRange> rand_ranges;
		*/
	};
}