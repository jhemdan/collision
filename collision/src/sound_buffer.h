#pragma once

#include "wav_file.h"

namespace jaw
{
	struct SoundBuffer
	{
		SoundBuffer();

		//throws exception on failure
		void create(const WavFile& wav_file);
		void destroy();

		unsigned id;
	};
}