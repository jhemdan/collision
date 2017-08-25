#include "portal.h"

namespace jaw
{
	Portal::Portal(Texture2d* tex)
	{
		sprite_g.create(tex);

		graphic = &sprite_g;

		sprite_g.frame_size = { 128, 128 };

		SpriteAnim anim = { "idle", {0, 1, 2, 3 }, 16, true };
		sprite_g.add_anim(anim);
		sprite_g.play_anim("idle");

		type = "portal";
		size = { 81, 106 };
		origin = { 24, 13 };
	}
}