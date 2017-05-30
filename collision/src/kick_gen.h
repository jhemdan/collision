#pragma once

#include "bitmap.h"

#include <vector>

namespace jaw
{
	struct KickWave
	{
		KickWave();

		void generate(float len);

		float get_value(float t) const;

		void gen_bmp(Bitmap& out_bmp) const;

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
	};
}