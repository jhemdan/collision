#pragma once

#include <string>

namespace jaw
{
	//throws exception on failure
	void init_log(const std::string& file_name);
	void log_write(const std::string& msg);
	void log_write(int i);
	void log_line(const std::string& line);
}