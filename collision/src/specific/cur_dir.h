#pragma once

namespace jaw
{
	enum CurDir
	{
		CUR_DIR_DOWN, CUR_DIR_UP, CUR_DIR_LEFT, CUR_DIR_RIGHT
	};

	CurDir cur_dir_from_angle(float angle);
}