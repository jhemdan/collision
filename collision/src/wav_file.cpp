#include "wav_file.h"
#include "load_file.h"
#include "log.h"

namespace jaw
{
	WavFile::WavFile()
	{
		num_channels = 0;
		sample_rate = 0;
		bits_per_sample = 0;
	}

	bool WavFile::create(const std::string& file_name)
	{
		std::string file_content;
		if (!load_file(file_name, file_content))
		{
			return false;
		}

		const char* cstr = file_content.data();
		uint32 ubuff32;
		uint16 ubuff16;

		if (strncmp(cstr, "RIFF", 4) == 0)
		{
			cstr += 4;

			//skip ChunkSize
			cstr += 4;

			if (strncmp(cstr, "WAVE", 4) == 0)
			{
				cstr += 4;

				if (strncmp(cstr, "fmt ", 4) == 0)
				{
					cstr += 4;

					//assumes little endian machine
					memcpy(&ubuff32, cstr, 4);

					//Subchunk1Size should be 16 for PCM
					if (ubuff32 == 16)
					{
						cstr += 4;

						//AudioFormat
						memcpy(&ubuff16, cstr, 2);

						//should be 1 for PCM
						if (ubuff16 == 1)
						{
							cstr += 2;

							//NumChannels
							memcpy(&ubuff16, cstr, 2);

							num_channels = ubuff16;

							if (num_channels == 1 || num_channels == 2)
							{
								cstr += 2;

								//SampleRate
								memcpy(&ubuff32, cstr, 4);

								sample_rate = (int)ubuff32;
								cstr += 4;

								//skip ByteRate
								cstr += 4;
								//skip BlockAlign
								cstr += 2;

								//BitsPerSample
								memcpy(&ubuff16, cstr, 2);

								bits_per_sample = (int)ubuff16;

								cstr += 2;

								if (strncmp(cstr, "data", 4) == 0)
								{
									cstr += 4;

									//Subchunk2Size
									memcpy(&ubuff32, cstr, 4);

									unsigned data_size = (unsigned)ubuff32;
									data.resize(data_size);

									cstr += 4;

									memcpy(data.data(), cstr, data_size);

									return true;
								}
							}
						}
					}
				}
			}
		}

		*this = {};
		
		log_write("Bad WAV file \"" + file_name + "\"");

		return false;
	}
}