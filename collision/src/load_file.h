#pragma once

#include <string>

namespace jaw
{
	//uses log on errors
	bool load_file(const std::string& path, std::string& out_content);
}