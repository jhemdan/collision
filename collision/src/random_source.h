#pragma once

#include <random>

namespace jaw
{
	struct RandomSource
	{
		RandomSource();

		float get();
		float get(float max);
		float get(float min, float max);

		std::mt19937 mt_random;
		std::uniform_real_distribution<float> random_dist;
	};
}