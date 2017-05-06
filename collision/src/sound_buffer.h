#pragma once

#include "wav_file.h"

namespace jaw
{
	struct SoundBuffer
	{
		SoundBuffer();

		//throws exception on failure
		void create(int num_channels, int sample_rate, int bits_per_sample, const ubyte* data, unsigned data_size);
		//throws exception on failure
		void create(const WavFile& wav_file);
		
		void destroy();

		unsigned id;
	};
}