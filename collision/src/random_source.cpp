#include "random_source.h"

namespace jaw
{
	RandomSource::RandomSource()
	{
		std::random_device random_device;
		mt_random.seed(random_device());
	}

	float RandomSource::get()
	{
		return random_dist(mt_random);
	}

	float RandomSource::get(float max)
	{
		return get() * max;
	}

	float RandomSource::get(float min, float max)
	{
		return min + (get() * (max - min));
	}
}