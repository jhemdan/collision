#pragma once

namespace jaw
{
	struct SoundSource
	{
		SoundSource();

		//throws exception on failure
		void create();
		void destroy();

		void queue_buffer(unsigned buff_id);

		void play();

		unsigned id;
	};
}