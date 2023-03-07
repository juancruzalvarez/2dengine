#ifndef SHADER_H_
#define SHADER_H_

#include <stdint.h>
#include <string>

#include <glm.hpp>

class Shader
{
public:
	Shader() {};
	Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path = nullptr);
	void use();
	void set_bool(std::string name, bool value);
	void set_int(std::string name, int value);
	void set_float(std::string name, float value);
	void set_v2f(std::string name, glm::vec2 value);
	void set_v3f(std::string name, glm::vec3 value);
	void set_v4f(std::string name, glm::vec4 value);
	void set_m4x4f(std::string name, glm::mat4 value);
	~Shader();

private:
	uint32_t program_id_;
	std::string read_file_to_str(const char* path);
	uint32_t create_shader_program(std::string source_code, int shader_type);
};

#endif
