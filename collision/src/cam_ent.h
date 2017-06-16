#pragma once

#include "entity.h"

#include <vecmath/matrix.hpp>

namespace jaw
{
	struct CamEnt : Entity
	{
		CamEnt();

		vcm::mat4 get_cam_tran() const;

		vcm::vec2 size;
	};
}