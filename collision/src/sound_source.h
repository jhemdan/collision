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

		void set_loop(bool val);

		void play();
		void stop();

		unsigned id;
	};
}