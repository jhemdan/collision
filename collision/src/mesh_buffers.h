#pragma once

#include "mesh.h"

namespace jaw
{
	struct MeshBuffers
	{
		MeshBuffers();

		void _set_data(const Mesh& mesh);

		//throws exception on failure
		void create(const Mesh& mesh, bool streaming = false);
		//throws exception on failure
		void recreate(const Mesh& mesh);

		void destroy();

		unsigned vbo, ibo;
		std::vector<float> vdata;
		std::vector<unsigned> idata;
		unsigned vsize;
		unsigned isize; //use this when rendering because the buffer can be bigger than what should be drawn
		bool streaming;
	};
}