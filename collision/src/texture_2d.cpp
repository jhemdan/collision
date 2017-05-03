#include "texture_2d.h"
#include "log.h"
#include "exception.h"

#include <GL/glew.h>
#include <assert.h>

namespace jaw
{
	Texture2d::Texture2d()
	{
		id = 0;
		w = h = 0;
		filter = TEX_2D_FILTER_UNDEFINED;
		wrap = TEX_2D_WRAP_UNDEFINED;
		format = TEX_2D_FORMAT_UNDEFINED;
	}

	void Texture2d::create(const Bitmap& bitmap, Tex2dFilter filter, Tex2dWrap wrap)
	{
		GLenum min_gl, mag_gl, wrap_gl;
		GLenum format_gl;

		switch (filter)
		{
		case TEX_2D_FILTER_LINEAR:
			min_gl = GL_LINEAR;
			mag_gl = GL_LINEAR;
			break;
		default:
			assert(false);
		case TEX_2D_FILTER_NEAREST:
			min_gl = GL_NEAREST;
			mag_gl = GL_NEAREST;
			break;
		}

		switch (wrap)
		{
		case TEX_2D_WRAP_CLAMP:
			wrap_gl = GL_CLAMP_TO_EDGE;
			break;
		default:
			assert(false);
		case TEX_2D_WRAP_REPEAT:
			wrap_gl = GL_REPEAT;
			break;
		}

		Tex2dFormat tex_fmt = TEX_2D_FORMAT_UNDEFINED;
		switch (bitmap.format)
		{
		case BITMAP_GRAY:
			format_gl = GL_RED;
			tex_fmt = TEX_2D_FORMAT_R;
			break;
		case BITMAP_RGB:
			format_gl = GL_RGB;
			tex_fmt = TEX_2D_FORMAT_RGB;
			break;
		default:
			assert(false);
		case BITMAP_RGBA:
			format_gl = GL_RGBA;
			tex_fmt = TEX_2D_FORMAT_RGBA;
			break;
		}

		this->w = bitmap.w;
		this->h = bitmap.h;
		this->format = tex_fmt;
		this->filter = filter;
		this->wrap = wrap;

		glGenTextures(1, &this->id);

		glBindTexture(GL_TEXTURE_2D, this->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_gl);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_gl);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_gl);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_gl);

		glTexImage2D(GL_TEXTURE_2D, 0, format_gl, this->w, this->h, 0, format_gl, GL_UNSIGNED_BYTE, bitmap.data.data());

		int gl_error = glGetError();
		if (gl_error || !this->id)
		{
			log_write("Failed to create texture. OpenGL error: ");
			log_write(gl_error);
			log_write("\n");

			destroy();

			throw Exception("Could not create texture.");
		}
	}

	void Texture2d::destroy()
	{
		glDeleteTextures(1, &id);
		
		*this = {};
	}
}