#include "shader.h"
#include "load_file.h"
#include "log.h"

#include <GL/glew.h>
#include <assert.h>

namespace jaw
{
	ShaderPart::ShaderPart()
	{
		id = 0;
		type = SHADER_UNDEFINED;
	}

	bool ShaderPart::create(const std::string& file_name, ShaderType type)
	{
		std::string src_code;
		if (!load_file(file_name, src_code))
		{
			return false;
		}

		assert(type != SHADER_UNDEFINED);
		GLenum gl_type = GL_VERTEX_SHADER;
		if (type == SHADER_FRAGMENT)
			gl_type = GL_FRAGMENT_SHADER;

		const char* c_src = src_code.c_str();
		unsigned shader = glCreateShader(gl_type);
		glShaderSource(shader, 1, &c_src, nullptr);
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			char log_buff[1024];
			glGetShaderInfoLog(shader, sizeof(log_buff), nullptr, log_buff);
			glDeleteShader(shader);

			log_line("Shader part \"" + file_name + "\" COMPILE ERROR: " + std::string(log_buff));
			return false;
		}

		this->id = shader;
		this->type = type;
		this->file_name = file_name;

		return true;
	}

	void ShaderPart::destroy()
	{
		glDeleteShader(id);
		id = 0;
		type = SHADER_UNDEFINED;
		file_name = "";
	}

	Shader::Shader()
	{
		id = 0;
	}

	bool Shader::create(const ShaderPart& vert, const ShaderPart& frag, const ShaderAttribLocs& attribs)
	{
		unsigned program = glCreateProgram();
		glAttachShader(program, vert.id);
		glAttachShader(program, frag.id);
		
		for (const auto& pair : attribs.locs)
		{
			glBindAttribLocation(program, pair.second, pair.first.c_str());
		}

		glLinkProgram(program);
		glDetachShader(program, vert.id);
		glDetachShader(program, frag.id);

		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (!status)
		{
			char log_buff[1024];
			glGetProgramInfoLog(program, sizeof(log_buff), nullptr, log_buff);
			glDeleteProgram(program);

			log_line("Shader program \"" + vert.file_name + "\" & \"" + frag.file_name + "\" LINK ERROR: " + std::string(log_buff));
			return false;
		}

		this->id = program;

		GLint num_attribs, num_uniforms;
		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &num_attribs);
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &num_uniforms);

		char name_buff[64];
		GLsizei name_length;
		GLint var_size;
		GLenum var_type;

		for (GLint i = 0; i < num_attribs; ++i)
		{
			glGetActiveAttrib(program, i, sizeof(name_buff), &name_length, &var_size, &var_type, name_buff);

			std::string attrib_name = name_buff;
			//get rid of [0] at the end of array names
			attrib_name = attrib_name.substr(0, attrib_name.find('['));

			this->attribs.locs.push_back(std::make_pair(attrib_name, glGetAttribLocation(program, name_buff)));
		}

		for (GLint i = 0; i < num_uniforms; ++i)
		{
			glGetActiveUniform(program, i, sizeof(name_buff), &name_length, &var_size, &var_type, name_buff);

			std::string uniform_name = name_buff;
			//get rid of [0] at the end of array names
			uniform_name = uniform_name.substr(0, uniform_name.find('['));

			this->uniforms.locs.push_back(std::make_pair(uniform_name, glGetUniformLocation(program, name_buff)));
		}

		return true;
	}

	bool Shader::create(const std::string& vert_path, const std::string& frag_path, const ShaderAttribLocs& attribs)
	{
		ShaderPart vert, frag;
		if (!vert.create(vert_path, SHADER_VERTEX))
		{
			return false;
		}
		if (!frag.create(frag_path, SHADER_FRAGMENT))
		{
			vert.destroy();
			return false;
		}

		bool c = create(vert, frag, attribs);
		vert.destroy();
		frag.destroy();

		return c;
	}

	void Shader::destroy()
	{
		glDeleteProgram(id);
		this->id = 0;
		this->attribs = this->uniforms = {};
	}
}