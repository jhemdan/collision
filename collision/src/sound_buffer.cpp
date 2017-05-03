#include "sound_buffer.h"
#include "log.h"
#include "exception.h"

#include <al.h>

namespace jaw
{
	SoundBuffer::SoundBuffer()
	{
		id = 0;
	}

	void SoundBuffer::create(const WavFile& wav_file)
	{
		alGenBuffers(1, &this->id);

		ALenum al_format;
		if (wav_file.num_channels == 2)
		{
			if (wav_file.bits_per_sample == 8)
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
			if (wav_file.bits_per_sample == 8)
			{
				al_format = AL_FORMAT_MONO8;
			}
			else
			{
				al_format = AL_FORMAT_MONO16;
			}
		}

		alBufferData(this->id, al_format, wav_file.data.data(), wav_file.data.size(), wav_file.sample_rate);

		int al_error = alGetError();
		if (al_error)
		{
			log_write("Could not create AL buffer for WAV file \"" + wav_file.file_name + "\". OpenAL error: ");
			log_write(al_error);
			log_write("\n");

			destroy();

			throw Exception("Could not create sound buffer.");
		}
	}

	void SoundBuffer::destroy()
	{
		alDeleteBuffers(1, &id);
		
		*this = {};
	}
}