#include "cur_dir.h"
#include "../jaw_macros.h"

#include <vecmath/pi.hpp>

namespace jaw
{
	CurDir cur_dir_from_angle(float angle)
	{
		CurDir cur_dir = CUR_DIR_DOWN;

		if (angle <= (vcm::PI / 4) || angle >= (7 * vcm::PI / 4))
		{
			cur_dir = CUR_DIR_RIGHT;
		}
		else if (angle <= (3 * vcm::PI / 4) && angle >= (vcm::PI / 4))
		{
			cur_dir = CUR_DIR_UP;
		}
		else if (angle <= (5 * vcm::PI / 4) && angle >= (3 * vcm::PI / 4))
		{
			cur_dir = CUR_DIR_LEFT;
		}
		else if (angle <= (7 * vcm::PI / 4) && angle >= (5 * vcm::PI / 4))
		{
			cur_dir = CUR_DIR_DOWN;
		}
		else
		{
			JAW_ASSERT_MSG(false, "Bad angle for cur_dir_from_angle()");
		}

		return cur_dir;
	}
}