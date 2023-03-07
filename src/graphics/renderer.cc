#include "renderer.hh"

#include "glad/glad.h"
#include <iostream>

#include <gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>

#include "util.hh"

namespace graphics {

        
const float Renderer::kQuadVertices [] = { 
    1.f, 1.f, 
    1.f, 0.f,
    0.f, 0.f,
    0.f, 1.f, 
};

const int Renderer::kQuadIndices[] = { 
    0, 1, 3,
    1, 2, 3, 
};

const int Renderer::kMaxQuads = 100;

Renderer::Renderer(): shader_("res/shaders/vertex.shader", "res/shaders/fragment.shader") {
    current_quad_ = 0;
    current_texture_id_ = 0;
    

    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);


    uint32_t vertex_vbo_id;
    glGenBuffers(1, &vertex_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), kQuadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    uint32_t indices_vbo_id;
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), kQuadIndices, GL_STATIC_DRAW);


    glGenBuffers(1, &instanced_data_vbo_id_);
    glBindBuffer(GL_ARRAY_BUFFER, instanced_data_vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, kMaxQuads * 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);


    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	uint8_t white_pixel_data[4] = {255,255,255,255};
	graphics::TextureCreateInfo info{
		GL_RGBA8,
		GL_NEAREST,
		GL_REPEAT,
		{1,1},
	};

	m_white_square_texture = graphics::Texture{ info,GL_RGBA, GL_UNSIGNED_BYTE, (void*)white_pixel_data };
}

void Renderer::Render(Quad quad, uint32_t texture_id) {
    float t[16] = { quad.pos.x, quad.pos.y, quad.size.x, quad.size.y, quad.uvs[0], quad.uvs[1],quad.uvs[2], quad.uvs[3],
    quad.uvs[4], quad.uvs[5],quad.uvs[6], quad.uvs[7], quad.color.r, quad.color.g, quad.color.b, quad.color.a };
    if (texture_id != current_texture_id_ || current_quad_ >= kMaxQuads) {
        Flush();
        current_texture_id_ = texture_id;
    }
    glBindBuffer(GL_ARRAY_BUFFER, instanced_data_vbo_id_);
    glBufferSubData(GL_ARRAY_BUFFER, current_quad_ * sizeof(t), sizeof(t), t);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    current_quad_++;
}

void Renderer::RenderWhiteQuad(Quad quad) {
	Render({quad.pos, quad.size, {1.f,1.f,1.f,0.f,0.f,0.f,0.f,1.f}, quad.color}, m_white_square_texture.texture_id);
}

void Renderer::LoadFont(std::string path, int pixel_height) {
	FT_Library  library;
	FT_Face face;
	auto error = FT_Init_FreeType(&library);
	if (error) {
		std::cout << "There was an error loading FT Library.\n";
		return;
	}
	error = FT_New_Face(library, path.c_str(), 0, &face);
	if (error) {
		std::cout << "There was an error loading the font.\n";
		return;
	}
	error = FT_Set_Pixel_Sizes(face, 0, pixel_height);
	if (error) {
		std::cout << "There was an error setting the font size.\n";
		return;
	}

	int tex_size = pixel_height * 16;
	auto bitmap_img = new uint8_t[tex_size * tex_size];
	int x = 0, y = 0;
	int current_max_height = 0;
	int current_max_width = 0;
	for (int r = 0; r < 16; r++) {
		for (int c = 0; c < 16; c++) {
			uint8_t current_char = r * 16 + c;
			error = FT_Load_Char(face, current_char, FT_LOAD_RENDER);
			if (error) continue;
			charmap_.insert({
				current_char,
				{
					glm::ivec2{x, y + 1},
					glm::ivec2{x + face->glyph->bitmap.width, y + face->glyph->bitmap.rows},
					glm::dvec2{face->glyph->bitmap.width, face->glyph->bitmap.rows},
					glm::dvec2{face->glyph->bitmap_left, face->glyph->bitmap_top},
					double(face->glyph->advance.x >> 6),
				}
				});
			if (face->glyph->bitmap.rows > current_max_height) {
				current_max_height = face->glyph->bitmap.rows;
			}
			if (face->glyph->bitmap.pitch > 0) {
				// bitmap buffer stores pixels in decreasing vertical position. First bytes in the buffer correspond to
				// the topmost pixels in the bitmap.
				for (int i = 0; i < charmap_[current_char].size.y; i++) {

					glm::ivec2 tex_pos = glm::ivec2(x, y + (charmap_[current_char].size.y - i));

					memcpy(
						&(bitmap_img[tex_pos.y * tex_size + tex_pos.x]),
						&(face->glyph->bitmap.buffer[i * face->glyph->bitmap.pitch]),
						charmap_[current_char].size.x
					);
				}
			}
			else {
				// bitmap buffer stores pixels in increasing vertical position. First bytes in the buffer correspond to
				// the bottom pixels in the bitmap.
				for (int i = 0; i < charmap_[current_char].size.y; i++) {

					glm::ivec2 tex_pos = glm::ivec2(x, y + i);
					memcpy(
						&(bitmap_img[tex_pos.y * tex_size + tex_pos.x]),
						&(face->glyph->bitmap.buffer[i * face->glyph->bitmap.pitch]),
						charmap_[current_char].size.x
					);

				}
			}
			x += face->glyph->bitmap.width;
			charmap_[current_char].size /= double(pixel_height);
			charmap_[current_char].bearing /= double(pixel_height);
			charmap_[current_char].advance /= double(pixel_height);
		}
		y += current_max_height;
		if (x > current_max_width)
			current_max_width = x;
		x = 0;

		m_font_info_max_under_baseline = 0.0;
		double max_over_baseline = 0.0;
		for (const auto& inf : charmap_) {
			max_over_baseline = std::max(max_over_baseline, inf.second.bearing.y);
			m_font_info_max_under_baseline = std::max(m_font_info_max_under_baseline, inf.second.size.y - inf.second.bearing.y);
		}
		std::cout << "Max over: " << max_over_baseline * pixel_height << "\n max_under: " << m_font_info_max_under_baseline *pixel_height << "\n";

	}

	graphics::TextureCreateInfo info{
		GL_RGBA8,
		GL_NEAREST,
		GL_REPEAT,
		{tex_size,tex_size},
	};

	font_texture_ = graphics::Texture(
		info,
		GL_RED,
		GL_UNSIGNED_BYTE,
		(void*)bitmap_img
	);

	delete[] bitmap_img;
}

void Renderer::RenderText(std::string text, glm::vec2 pos, int pixel_height, glm::vec4 color) {
	pos.y += m_font_info_max_under_baseline * pixel_height;
    for (const auto& c : text) {
		auto char_info = charmap_[c];
		double xpos = pos.x + char_info.bearing.x * pixel_height;
		double ypos = pos.y - (char_info.size.y - char_info.bearing.y) * pixel_height;
		Render(
			{ 
				{xpos, ypos},
				{char_info.size * double(pixel_height)},
				util::pixel_corners_to_uv_array(
					char_info.bot_left_corner,
					char_info.top_right_corner,
					font_texture_.size),
				color
			},
			font_texture_.texture_id
		);
		pos.x += char_info.advance * pixel_height;
    }
}

void RenderTextVerticallyCentered(std::string text, glm::vec2 pos, int pixel_height, glm::vec4 color) {

}




glm::vec2 Renderer::GetTextSize(std::string text, int pixel_height) {
	glm::vec2 size{ 0, pixel_height};

	for (const auto& c : text) {
		size.x += charmap_[c].advance * pixel_height;
		size.y = std::max(charmap_[c].size.y * pixel_height, (double)size.y * pixel_height);
	}
	return size;
}


void Renderer::Flush() {
    glBindVertexArray(vao_id_);
    shader_.use();
    glActiveTexture(GL_TEXTURE0);                   
    glBindTexture(GL_TEXTURE_2D, current_texture_id_);
    shader_.set_int("tex", 0);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, current_quad_);
    current_quad_ = 0;
}

void Renderer::SetOrtho(glm::vec2 size) {
    shader_.use();
    shader_.set_v2f("ortho_size", size);
}


}