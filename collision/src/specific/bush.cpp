#include "bush.h"

namespace jaw
{
	Bush::Bush(Texture2d* tex)
	{
		sprite_g.create(tex);
		sprite_g.origin = { 33, 58 };

		graphic = &sprite_g;

		solid = true;
		size = { 47, 42 };
		origin = Point{ 8, 18 } - sprite_g.origin;
	}

	Bush::~Bush()
	{
		sprite_g.destroy();
	}

	void Bush::on_added()
	{
		Entity::on_added();
	}

	void Bush::on_removed()
	{
		Entity::on_removed();
	}
}