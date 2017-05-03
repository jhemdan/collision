#pragma once

#include "types.h"

#include <vecmath/vector.hpp>
#include <vector>

namespace jaw
{
	struct MeshVertex
	{
		MeshVertex()
			: color(1.0f)
		{

		}

		MeshVertex(const vcm::vec2& position)
			: MeshVertex()
		{
			this->position = position;
		}

		//remember: vcm::vec2 can be created from single float
		MeshVertex(const vcm::vec2& position, const vcm::vec2& tex_coords)
			: MeshVertex(position)
		{
			this->tex_coords = tex_coords;
		}

		//remember: vcm::vec2 can be created from single float
		MeshVertex(const vcm::vec2& position, const vcm::vec2& tex_coords, const vcm::vec4& color)
			: MeshVertex(position, tex_coords)
		{
			this->color = color;
		}

		vcm::vec2 position;
		vcm::vec2 tex_coords;
		vcm::vec4 color;
	};

	struct MeshTriangle
	{
		MeshTriangle() { i[0] = i[1] = i[2] = 0; }

		MeshTriangle(unsigned a, unsigned b, unsigned c)
		{
			i[0] = a;
			i[1] = b;
			i[2] = c;
		}

		MeshTriangle operator+(unsigned offset) const
		{
			return { i[0] + offset, i[1] + offset, i[2] + offset };
		}

		unsigned i[3];
	};

	struct Mesh
	{
		Mesh();

		std::vector<MeshVertex> vertices;
		std::vector<MeshTriangle> triangles;
	};
}