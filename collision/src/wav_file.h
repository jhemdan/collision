#pragma once

#include "types.h"

#include <string>
#include <vector>

namespace jaw
{
	struct WavFile
	{
		WavFile();

		bool create(const std::string& file_name);
		
		int num_channels;
		int sample_rate;
		int bits_per_sample;
		std::vector<ubyte> data;
	};
}