#include "texture.hh"
#include <iostream>
namespace graphics {
	
	Texture::Texture(TextureCreateInfo texture_info, uint32_t data_format, uint32_t data_type, void* data) {

		glGenTextures(1, &texture_id);

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_info.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_info.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_info.filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_info.filter);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, texture_info.format, texture_info.width, texture_info.height, 0, data_format, data_type, data);

		format = texture_info.format;
		width = texture_info.width;
		height = texture_info.height;
	}
}
