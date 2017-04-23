#pragma once

#include "mesh.h"

namespace jaw
{
	struct MeshBuffers
	{
		MeshBuffers();

		void _set_data(const Mesh& mesh);
		void create(const Mesh& mesh, bool streaming = false);
		void recreate(const Mesh& mesh);
		void destroy();

		unsigned vbo, ibo;
		std::vector<float> vdata;
		std::vector<unsigned> idata;
		unsigned vsize;
		unsigned isize;
		bool streaming;
	};
}