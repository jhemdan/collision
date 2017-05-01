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
		this->pitch = multiple_of_four(this->w * bytes_per_pixel(this->format));

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
		w = h = 0;
		pitch = 0;
		format = BITMAP_UNDEFINED;
		data.clear();
	}
}