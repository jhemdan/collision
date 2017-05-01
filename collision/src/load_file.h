#pragma once

#include <string>

namespace jaw
{
	//uses log on errors
	//throws exception on failure
	void load_file(const std::string& path, std::string& out_content);
}