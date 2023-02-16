#include "shader.hh"

#include <stdint.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>


Shader::Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path)
{
	GLint success;
	GLchar infoLog[1024];
	std::string vertex_code = read_file_to_str(vertex_path);
	std::string fragment_code = read_file_to_str(fragment_path);
	uint32_t vertex_program = create_shader_program(vertex_code, GL_VERTEX_SHADER);
	uint32_t fragment_program = create_shader_program(fragment_code, GL_FRAGMENT_SHADER);
	uint32_t geometry_program;
	program_id_ = glCreateProgram();
	glAttachShader(program_id_, vertex_program);
	glAttachShader(program_id_, fragment_program);
	if (geometry_path)
	{
		std::string geometry_code = read_file_to_str(geometry_path);
		geometry_program = create_shader_program(geometry_code, GL_GEOMETRY_SHADER);
		glAttachShader(program_id_, geometry_program);

	}
	glLinkProgram(program_id_);
	glGetProgramiv(program_id_, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program_id_, 1024, NULL, infoLog);
		std::cout << "Unable to link shaders. Log:" << "\n" << infoLog << "\n";
	}

	glDeleteShader(vertex_program);
	glDeleteShader(fragment_program);
	if (geometry_path)
	{
		glDeleteShader(geometry_program);
	}

}

void Shader::use()
{
	glUseProgram(program_id_);
}

void Shader::set_bool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(program_id_, name.c_str()), (int)value);
}

void Shader::set_int(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(program_id_, name.c_str()), (int)value);
}
void Shader::set_float(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
}
void Shader::set_v3f(std::string name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
}
void Shader::set_v4f(std::string name, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
}
void Shader::set_m4x4f(std::string name, glm::mat4 value)
{
	std::cout << "loc:" << glGetUniformLocation(program_id_, "transform") << "\n";
	glUniformMatrix4fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

Shader::~Shader() 
{
	//glDeleteProgram(program_id_);
}

std::string Shader::read_file_to_str(const char* path)
{
	std::ifstream stream(path);
	if (!stream.is_open()) {
		std::cout << "Unable to open file: " << path << "\n";
		return "";
	}
	std::stringstream str_buffer;
	str_buffer << stream.rdbuf();
	std::string ret = str_buffer.str();
	stream.close();
	return ret;
}

uint32_t Shader::create_shader_program(std::string source_code, int shader_type) {
	uint32_t program;
	GLint success;
	GLchar infoLog[1024];
	const char* source_code_c = source_code.c_str();
	program = glCreateShader(shader_type);
	glShaderSource(program, 1, &source_code_c, NULL);
	glCompileShader(program);
	glGetShaderiv(program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Unable to compile shader. Error log:" << "\n" << infoLog << "\n";
	}
	return program;
}