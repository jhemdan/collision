#include "log.h"

#include <fstream>

namespace jaw
{
	static std::ofstream file;

	bool init_log(const std::string& name)
	{
		file.open(name, std::ios::out | std::ios::binary);
		return !file.bad();
	}

	void log_write(const std::string& msg)
	{
		file << msg;
	}

	void log_write(int i)
	{
		file << i;
	}

	void log_line(const std::string& line)
	{
		log_write(line);
		file << std::endl;
	}
}