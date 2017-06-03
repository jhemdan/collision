#include "sound_buffer.h"
#include "log.h"
#include "exception.h"
#include "jaw_macros.h"

#include <al.h>

namespace jaw
{
	SoundBuffer::SoundBuffer()
	{
		id = 0;
	}

	void SoundBuffer::create(int num_channels, int sample_rate, int bits_per_sample, const ubyte* data, unsigned data_size)
	{
		JAW_ASSERT_MSG(num_channels == 1 || num_channels == 2, "Bad num_channels for SoundBuffer::create()");
		JAW_ASSERT_MSG(bits_per_sample == 8 || bits_per_sample == 16, "Bad bits_per_sample for SoundBuffer::create()");

		alGenBuffers(1, &this->id);

		ALenum al_format;
		if (num_channels == 2)
		{
			if (bits_per_sample == 8)
			{
				al_format = AL_FORMAT_STEREO8;
			}
			else
			{
				al_format = AL_FORMAT_STEREO16;
			}
		}
		else
		{
			if (bits_per_sample == 8)
			{
				al_format = AL_FORMAT_MONO8;
			}
			else
			{
				al_format = AL_FORMAT_MONO16;
			}
		}

		alBufferData(this->id, al_format, data, data_size, sample_rate);

		int al_error = alGetError();
		if (al_error)
		{
			log_write("Could not create AL buffer. OpenAL error: ");
			log_write(al_error);
			log_write("\n");

			destroy();

			throw Exception("Could not create sound buffer.");
		}
	}

	void SoundBuffer::create(const WavFile& wav_file)
	{
		try
		{
			create(wav_file.num_channels, wav_file.sample_rate, wav_file.bits_per_sample, wav_file.data.data(), wav_file.data.size());
		}
		catch (const Exception& e)
		{
			log_line("Failed to create sound buffer for WAV file \"" + wav_file.file_name + "\"");

			throw e;
		}
	}

	void SoundBuffer::destroy()
	{
		alDeleteBuffers(1, &id);
		
		*this = {};
	}
}