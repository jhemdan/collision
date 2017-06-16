#include "cam_ent.h"

namespace jaw
{
	CamEnt::CamEnt()
	{
		size = { 800.0f, 600.0f };
	}

	vcm::mat4 CamEnt::get_cam_tran() const
	{
		vcm::mat4 m;
		m[3] = vcm::vec4((float)position.x, (float)position.y, 0.0f, 1.0f);

		float screen_w = 800.0f;
		float screen_h = 600.0f;

		float scale_x = size.x / screen_w;
		float scale_y = size.y / screen_h;

		m[0] *= scale_x;
		m[1] *= scale_y;

		return m;
	}
}