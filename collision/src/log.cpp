#include "log.h"
#include "exception.h"

#include <fstream>

namespace jaw
{
	static std::ofstream file;

	void init_log(const std::string& name)
	{
		file.open(name, std::ios::out | std::ios::binary);

		if (file.bad())
			throw Exception("Failed to init log.");
	}

	void log_write(const std::string& msg)
	{
		file << msg;
	}

	void log_write(int i)
	{
		file << i;
	}

	void log_write(float f)
	{
		file << f;
	}

	void log_line(const std::string& line)
	{
		log_write(line);
		file << std::endl;
	}
}