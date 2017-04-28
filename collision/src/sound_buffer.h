#pragma once

#include "wav_file.h"

namespace jaw
{
	struct SoundBuffer
	{
		SoundBuffer();

		void create(const WavFile& wav_file);
		void destroy();

		unsigned id;
	};
}