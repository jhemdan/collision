#include "player_hud.h"

#include "../world.h"
#include "../cam_ent.h"

namespace jaw
{
	PlayerHud::PlayerHud()
	{
		world = nullptr;

		_heart_count = 3;
	}

	void PlayerHud::init()
	{
		Bitmap heart_bmp;
		heart_bmp.create("../assets/heart.png");
		heart_tex.create(heart_bmp, TEX_2D_FILTER_NEAREST, TEX_2D_WRAP_CLAMP);

		hearts_g[0].create(&heart_tex);
		hearts_g[1] = hearts_g[0];
		hearts_g[2] = hearts_g[0];

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].graphic = &hearts_g[i];

			hearts[i].set_layer(LAYER_NUM);
		
			hearts[i].rel_position = Point(20 + i * 20, 20);
		}	
	}

	void PlayerHud::clean()
	{
		heart_tex.destroy();
	}

	void PlayerHud::add(World* w)
	{
		world = w;

		for (int i = 0; i < 3; ++i)
		{
			hearts[i].parent = world->cam_ent;
		}

		for (int i = 0; i < 3; ++i)
		{
			world->add_entity(&hearts[i]);
		}

		world->add_entity(&heart_text_ent);
	}

	void PlayerHud::remove()
	{
		for (int i = 0; i < 3; ++i)
		{
			hearts[i].parent = nullptr;
		}

		for (int i = 0; i < 3; ++i)
		{
			world->remove_entity(&hearts[i]);
		}

		world->remove_entity(&heart_text_ent);

		world = nullptr;
	}

	void PlayerHud::set_hearts(int hc)
	{
		if (hc < 0)
			hc = 0;
		if (hc > 3)
			hc = 3;

		if (_heart_count != hc)
		{
			_heart_count = hc;

			for (int i = 0; i < 3; ++i)
				hearts_g[i].visible = false;
			for (int i = 0; i < hc; ++i)
				hearts_g[i].visible = true;
		}
	}
}