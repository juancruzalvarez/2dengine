#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

#include <glad/glad.h>

namespace graphics {

struct TextureCreateInfo {
	uint32_t format;
	uint32_t filter;
	uint32_t wrapping;
	int width, height;
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
int width, height;

};


}

#endif