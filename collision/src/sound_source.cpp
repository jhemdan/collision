#include "sound_source.h"
#include "log.h"
#include "exception.h"

#include <al.h>

namespace jaw
{
	SoundSource::SoundSource()
	{
		id = 0;
	}

	void SoundSource::create()
	{
		alGenSources(1, &this->id);

		int al_error = alGetError();
		if (al_error)
		{
			log_write("Failed to create AL source. OpenAL error: ");
			log_write(al_error);
			log_write("\n");

			destroy();

			throw Exception("Could not create sound source.");
		}
	}

	void SoundSource::destroy()
	{
		alDeleteSources(1, &id);
		id = 0;
	}

	void SoundSource::queue_buffer(unsigned buff_id)
	{
		alSourceQueueBuffers(id, 1, &buff_id);
	}

	void SoundSource::play()
	{
		alSourcePlay(id);
	}
}