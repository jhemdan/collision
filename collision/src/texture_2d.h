#pragma once

#include "bitmap.h"

namespace jaw
{
	enum Tex2dFilter
	{
		TEX_2D_FILTER_UNDEFINED,
		TEX_2D_FILTER_LINEAR,
		TEX_2D_FILTER_NEAREST
	};

	enum Tex2dWrap
	{
		TEX_2D_WRAP_UNDEFINED,
		TEX_2D_WRAP_REPEAT,
		TEX_2D_WRAP_CLAMP
	};

	enum Tex2dFormat
	{
		TEX_2D_FORMAT_UNDEFINED,
		TEX_2D_FORMAT_R,
		TEX_2D_FORMAT_RGB,
		TEX_2D_FORMAT_RGBA
	};

	struct Texture2d
	{
		Texture2d();

		//throws exception on failure
		void create(const Bitmap& bitmap, Tex2dFilter filter, Tex2dWrap wrap);
		void destroy();

		unsigned id;
		int w, h;
		Tex2dFilter filter;
		Tex2dWrap wrap;
		Tex2dFormat format;
	};
}