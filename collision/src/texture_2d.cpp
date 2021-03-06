#include "texture_2d.h"
#include "log.h"
#include "exception.h"
#include "jaw_macros.h"

#include <GL/glew.h>

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
		JAW_ASSERT_MSG(bitmap.w > 0 && bitmap.h > 0, "Bad bitmap size for Texture2d::create()");
		JAW_ASSERT_MSG(bitmap.data.size(), "Bad bitmap data for Texture2d::create()");
		JAW_ASSERT_MSG(bitmap.pitch, "Bad bitmap pitch for Texture2d::create()");

		GLenum min_gl, mag_gl, wrap_gl;
		GLenum format_gl;

		switch (filter)
		{
		case TEX_2D_FILTER_LINEAR:
			min_gl = GL_LINEAR;
			mag_gl = GL_LINEAR;
			break;
		default:
			JAW_ASSERT_MSG(false, "Bad filter for Texture2d::create()");
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
			JAW_ASSERT_MSG(false, "Bad wrap for Texture2d::create()");
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
			JAW_ASSERT_MSG(false, "Bad bitmap format for Texture2d::create()");
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
			log_write("Failed to create texture2d. OpenGL error: ");
			log_write(gl_error);
			log_write("\n");

			destroy();

			throw Exception("Could not create texture2d.");
		}
	}

	void Texture2d::recreate(const Bitmap& bitmap)
	{
		JAW_ASSERT_MSG(id, "No id in Texture2d::recreate()");

		GLenum format_gl;

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
			JAW_ASSERT_MSG(false, "Bad bitmap format in Texture2d::recreate()");
		case BITMAP_RGBA:
			format_gl = GL_RGBA;
			tex_fmt = TEX_2D_FORMAT_RGBA;
			break;
		}

		Tex2dFormat old_fmt = this->format;
		int old_w = this->w;
		int old_h = this->h;

		this->w = bitmap.w;
		this->h = bitmap.h;
		this->format = tex_fmt;

		glBindTexture(GL_TEXTURE_2D, this->id);

		if (old_fmt != this->format || old_w != this->w || old_h != this->h)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format_gl, this->w, this->h, 0, format_gl, GL_UNSIGNED_BYTE, bitmap.data.data());
		}
		else
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->w, this->h, format_gl, GL_UNSIGNED_BYTE, bitmap.data.data());
		}

		int gl_error = glGetError();
		if (gl_error || !this->id)
		{
			log_write("Failed to recreate texture2d. OpenGL error: ");
			log_write(gl_error);
			log_write("\n");

			destroy();

			throw Exception("Could not recreate texture2d.");
		}
	}

	void Texture2d::destroy()
	{
		glDeleteTextures(1, &id);
		
		*this = {};
	}
}