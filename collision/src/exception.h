#pragma once

#include <string>

namespace jaw
{
	struct Exception
	{
		Exception() {}
		Exception(const std::string& msg) : msg(msg) {}

		std::string msg;
	};
}