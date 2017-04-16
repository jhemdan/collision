#include "mesh_buffers.h"

#include <GL/glew.h>

namespace jaw
{
	MeshBuffers::MeshBuffers()
	{
		vbo = ibo = 0;
	}

	void MeshBuffers::create(const Mesh& mesh)
	{
		vdata.resize(mesh.vertices.size() * 8);
		idata.resize(mesh.triangles.size() * 3);

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

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ARRAY_BUFFER, vdata.size() * sizeof(float), vdata.data(), GL_STATIC_DRAW); //only add stream if too slow
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idata.size() * sizeof(unsigned), idata.data(), GL_STATIC_DRAW);
	}

	void MeshBuffers::destroy()
	{
		if (vbo)
		{
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ibo);
			vbo = ibo = 0;
		}

		vdata.clear();
		idata.clear();
	}
}