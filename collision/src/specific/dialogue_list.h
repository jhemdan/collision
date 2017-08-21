#pragma once

#include <vector>
#include <string>

namespace jaw
{
	struct DialogueList
	{
		std::vector<std::string> list;
		std::vector<int> codes;

		void add(const std::string& msg, int code)
		{
			list.push_back(msg);
			codes.push_back(code);
		}
	};
}