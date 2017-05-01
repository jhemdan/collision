#include "load_file.h"
#include "log.h"
#include "exception.h"

#include <fstream>

namespace jaw
{
	void load_file(const std::string& path, std::string& out_content)
	{
		std::ifstream file;
		file.open(path, std::ios::in | std::ios::binary | std::ios::ate);
		if (!file)
		{
			log_line("Couldn't open file \"" + path + "\"");

			throw Exception("Couldn't open file for load_file()");
		}

		unsigned size = (unsigned)file.tellg();
		file.seekg(0, file.beg);

		std::string buff;
		buff.resize(size);
		file.read(&buff[0], size);

		if (file.bad())
		{
			log_line("Failed to read file \"" + path + "\"");

			throw Exception("Failed to read file for load_file()");
		}

		out_content = buff;
	}
}