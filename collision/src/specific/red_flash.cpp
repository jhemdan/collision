#include "red_flash.h"
#include "../jaw_macros.h"

namespace jaw
{
	RedFlash::RedFlash()
	{
		flashing = false;
		timer = 0.0f;
		sprite = nullptr;
	}

	void RedFlash::hit()
	{
		flashing = true;
		timer = 0.0f;
	}

	void RedFlash::update(float dt)
	{
		if (!sprite)
			JAW_ASSERT_MSG(false, "No sprite for RedFlash");

		if (sprite)
		{
			sprite->mix_color_amount = 0.0f;
			sprite->color = vcm::vec4{ 1.0f };
		}
		
		if (flashing)
		{
			timer += dt;

			int redi = (int)((timer * 4.0f) * 1000);
			int redi2 = redi % 1000;

			if (redi <= 1000)
			{
				if (redi2 < 333)
				{
					if (sprite)
					{
						sprite->mix_color = { 1.0f, 0.0f, 0.0f, 1.0f };
						sprite->mix_color_amount = .75f;
					}
				}
			}

			if (redi2 >= 333 && redi2 < 500)
			{
				if (sprite)
				{
					sprite->mix_color_amount = 0.0f;
					sprite->color = vcm::vec4{ 0.0f };
				}
			}

			if (redi > 3000)
			{
				flashing = false;
				timer = 0.0f;
			}
		}
	}
}