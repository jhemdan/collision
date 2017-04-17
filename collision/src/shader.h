#pragma once

#include <string>
#include <vector>

namespace jaw
{
	enum ShaderType
	{
		SHADER_UNDEFINED,
		SHADER_VERTEX,
		SHADER_FRAGMENT
	};

	struct ShaderPart
	{
		ShaderPart();

		//remember: doesn't destroy so don't call create() multiple times without calling destroy() first
		bool create(const std::string& file_name, ShaderType type);
		void destroy();

		unsigned id;
		ShaderType type;
		std::string file_name;
	};

	struct ShaderAttribLocs
	{
		ShaderAttribLocs() {}
		ShaderAttribLocs(std::initializer_list<std::pair<std::string, int>> locs)
		{
			this->locs = locs;
		}

		std::vector<std::pair<std::string, int>> locs;
	};

	using ShaderUniformLocs = ShaderAttribLocs;

	struct Shader
	{
		Shader();

		bool create(const ShaderPart& vert, const ShaderPart& frag, const ShaderAttribLocs& attribs);
		bool create(const std::string& vert_path, const std::string& frag_path, const ShaderAttribLocs& attribs);
		void destroy();

		int get_uniform(const std::string& name) const;
		int get_attribute(const std::string& name) const;

		unsigned id;
		ShaderAttribLocs attribs;
		ShaderUniformLocs uniforms;
	};
}