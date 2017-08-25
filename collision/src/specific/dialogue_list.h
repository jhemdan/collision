#pragma once

#include <vector>
#include <string>

namespace jaw
{
	struct DialogueList
	{
		std::vector<std::string> list;
		std::vector<int> codes;

		int next_dialogue_state; //-1 for N/A

		DialogueList() : next_dialogue_state(-1) {}

		void add(const std::string& msg, int code)
		{
			list.push_back(msg);
			codes.push_back(code);
		}
	};
}