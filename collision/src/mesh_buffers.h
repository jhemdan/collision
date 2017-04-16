#pragma once

#include "mesh.h"

namespace jaw
{
	struct MeshBuffers
	{
		MeshBuffers();

		void create(const Mesh& mesh);
		void destroy();

		unsigned vbo, ibo;
		std::vector<float> vdata;
		std::vector<unsigned> idata;
	};
}