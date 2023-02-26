#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

#include <glad/glad.h>
#include <glm.hpp>

namespace graphics {

struct TextureCreateInfo {
	uint32_t format;
	uint32_t filter;
	uint32_t wrapping;
	glm::ivec2 size;
};

class Texture {

public:

Texture() {};

Texture(
	TextureCreateInfo info,
	uint32_t data_format,
	uint32_t data_type,
	void* data
);

uint32_t texture_id;
uint32_t format;
glm::ivec2 size;
};


}

#endif