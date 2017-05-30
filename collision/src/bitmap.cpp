#include "bitmap.h"
#include "log.h"
#include "exception.h"

#include <SDL2/SDL_image.h>
#include <assert.h>

namespace jaw
{
	static int multiple_of_four(int val)
	{
		int rem = val % 4;
		if (rem > 0)
		{
			return val + (4 - rem);
		}

		return val;
	}

	static int bytes_per_pixel(BitmapFormat fmt)
	{
		switch (fmt)
		{
		case BITMAP_GRAY:
			return 1;
		case BITMAP_RGB:
			return 3;
		case BITMAP_RGBA:
			return 4;
		default:
			assert(false);
			return 0;
		}
	}

	Bitmap::Bitmap()
	{
		w = h = 0;
		pitch = 0;
		format = BITMAP_UNDEFINED;
	}

	void Bitmap::create(const std::string& file_name)
	{
		auto surface = IMG_Load(file_name.c_str());
		if (!surface)
		{
			log_line("Error loading image file \"" + file_name + "\": " + IMG_GetError());
			
			throw Exception("Failed to load image for bitmap.");
		}

		BitmapFormat format = BITMAP_UNDEFINED;
		switch (surface->format->BytesPerPixel)
		{
		case 1:
			format = BITMAP_GRAY;
			break;
		case 3:
			format = BITMAP_RGB;
			break;
		case 4:
			format = BITMAP_RGBA;
			break;
		default:
			log_write("Image \"" + file_name + "\" has invalid number of bytes per pixel (");
			log_write(surface->format->BytesPerPixel);
			log_write(")\n");

			SDL_FreeSurface(surface);

			throw Exception("Failed to create Bitmap.");
			break;
		}

		this->w = surface->w;
		this->h = surface->h;
		this->format = format;
		calc_pitch();

		this->data.resize(this->pitch * this->h);

		auto surf_data = (ubyte*)surface->pixels;

		int least_pitch = this->pitch;
		if (surface->pitch < this->pitch)
			least_pitch = surface->pitch;

		for (int i = 0; i < this->h; ++i)
		{
			memcpy(&this->data[i * this->pitch], &surf_data[/*(this->h - i - 1)*/i * surface->pitch], least_pitch);
		}

		SDL_FreeSurface(surface);
	}

	void Bitmap::destroy()
	{
		*this = {};
	}

	void Bitmap::calc_pitch()
	{
		this->pitch = multiple_of_four(this->w * bytes_per_pixel(this->format));
	}

	void Bitmap::set_pixel(int x, int y, unsigned value)
	{
		assert(!data.empty());

		if (x < 0 || y < 0)
			return;
		if (x >= w || y >= h)
			return;

		int bpp = bytes_per_pixel(format);
		int i = y * pitch + x * bpp;

		data[i] = value & 0x000000ff; //gray/r
		if (bpp >= 3)
		{
			data[i + 1] = (value & 0x0000ff00) >> 8; //g
			data[i + 2] = (value & 0x00ff0000) >> 16; //b
			if (bpp == 4)
			{
				data[i + 3] = (value & 0xff000000) >> 24; //a
			}
		}
	}

	void Bitmap::draw_line(int x1, int y1, int x2, int y2, unsigned value)
	{
		int diffx = x2 - x1;

		if (diffx != 0)
		{
			float slope = (float)(y2 - y1) / diffx;
			int sign = diffx > 0 ? 1 : (diffx < 0 ? -1 : 0);
			int x = x1;

			bool has_last_y = false;
			int last_y;

			for (;;)
			{
				int y = y1 + (int)round((x - x1) * slope);

				if (has_last_y && y - last_y != 0)
				{
					int last_y_sign = y - last_y;
					last_y_sign = last_y_sign < 0 ? -1 : (last_y_sign > 0 ? 1 : 0);

					int z = last_y + last_y_sign;
					for (;;)
					{
						set_pixel(x, z, value);

						if (z == y)
							break;

						z += last_y_sign;
					}
				}
				else
				{
					set_pixel(x, y, value);
				}

				if (x == x2)
					break;

				x += sign;

				last_y = y;
				has_last_y = true;
			}
		}
		else
		{
			int diffy = y2 - y1;
			int sign = diffy > 0 ? 1 : (diffy < 0 ? -1 : 0);
			int y = y1;
			for (;;)
			{
				int x = x1;
				set_pixel(x, y, value);
				if (y == y2)
					break;
				y += sign;
			}
		}
	}
}