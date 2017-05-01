#include "mesh_buffers.h"
#include "log.h"
#include "exception.h"

#include <GL/glew.h>

namespace jaw
{
	MeshBuffers::MeshBuffers()
	{
		vbo = ibo = 0;
		vsize = isize = 0;
		streaming = false;
	}

	void MeshBuffers::_set_data(const Mesh& mesh)
	{
		int i = 0;
		for (const auto& vertex : mesh.vertices)
		{
			vdata[i++] = vertex.position.x;
			vdata[i++] = vertex.position.y;

			vdata[i++] = vertex.tex_coords.x;
			vdata[i++] = vertex.tex_coords.y;

			vdata[i++] = vertex.color.x;
			vdata[i++] = vertex.color.y;
			vdata[i++] = vertex.color.z;
			vdata[i++] = vertex.color.w;
		}

		i = 0;
		for (const auto& triangle : mesh.triangles)
		{
			idata[i++] = triangle.i[0];
			idata[i++] = triangle.i[1];
			idata[i++] = triangle.i[2];
		}
	}

	void MeshBuffers::create(const Mesh& mesh, bool streaming)
	{
		vsize = mesh.vertices.size() * 8;
		isize = mesh.triangles.size() * 3;

		vdata.resize(vsize);
		idata.resize(isize);

		_set_data(mesh);

		this->streaming = streaming;

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(float), vdata.data(), streaming ? GL_STREAM_DRAW : GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(unsigned), idata.data(), streaming ? GL_STREAM_DRAW : GL_STATIC_DRAW);

		int gl_error = glGetError();
		if (gl_error || !vbo || !ibo)
		{
			log_write("Failed to create mesh buffers. OpenGL error: ");
			log_write(gl_error);
			log_write("\n");

			destroy();

			throw Exception("Failed to create mesh buffers.");
		}
	}

	void MeshBuffers::recreate(const Mesh& mesh)
	{
		vsize = mesh.vertices.size() * 8;
		isize = mesh.triangles.size() * 3;

		bool bigger_v = false;
		bool bigger_i = false;

		if (vsize > vdata.size())
		{
			vdata.resize(vsize);
			bigger_v = true;
		}
		if (isize > idata.size())
		{
			idata.resize(isize);
			bigger_i = true;
		}

		_set_data(mesh);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		if (bigger_v)
		{
			glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(float), vdata.data(), streaming ? GL_STREAM_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, vsize * sizeof(float), vdata.data());
		}

		if (bigger_i)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(unsigned), idata.data(), streaming ? GL_STREAM_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, isize * sizeof(unsigned), idata.data());
		}

		int gl_error = glGetError();
		if (gl_error)
		{
			log_write("Failed to recreate mesh buffers. OpenGL error: ");
			log_write(gl_error);
			log_write("\n");

			destroy();

			throw Exception("Failed to recreate mesh buffers.");
		}
	}

	void MeshBuffers::destroy()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		vbo = ibo = 0;
		vdata.clear();
		idata.clear();
		vsize = isize = 0;
		streaming = false;
	}
}