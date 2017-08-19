#pragma once

#include <string>

namespace jaw
{
	struct TextBoxTree
	{
		TextBoxTree()
			: yes_no(false)
			, yes_tree(nullptr)
			, no_tree(nullptr)
			, next(nullptr)
			, result(-1)
		{

		}

		std::string msg;
		bool yes_no;
		TextBoxTree* yes_tree;
		TextBoxTree* no_tree;
		TextBoxTree* next;
		int result; //-1 if N/A

		void delete_all()
		{
			if (yes_tree)
				yes_tree->delete_all();
			if (no_tree)
				no_tree->delete_all();
			if (next)
				next->delete_all();

			delete yes_tree;
			delete no_tree;
			delete next;
		}
	};
}