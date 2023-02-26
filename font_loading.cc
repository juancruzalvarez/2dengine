#include "font_loading.hh"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace graphics {

void LoadFont(
	std::string path,
	int pixel_height,
	uint8_t** bitmap_img,
	glm::ivec2* bitmap_img_size,
	std::map<uint8_t, CharInfo>* charmap) {
	std::cout << "1\n";
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
	
	(*bitmap_img) = new uint8_t[tex_size * tex_size];
	int x = 0, y = 0;
	int current_max_height = 0;
	int current_max_width = 0;
	for (int r = 0; r < 16; r++) {
		for (int c = 0; c < 16; c++) {
			uint8_t current_char = r * 16 + c;
			error = FT_Load_Char(face, current_char, FT_LOAD_RENDER);
			if (error) continue;
			std::cout << "2\n";
			charmap->insert({
				current_char,
				{
					{x, y}, 
					{x+face->glyph->bitmap.width, y+face->glyph->bitmap.rows},
					{face->glyph->metrics.width, face->glyph->metrics.height},
					{face->glyph->metrics.horiBearingX, face->glyph->metrics.horiBearingY},
					face->glyph->metrics.horiAdvance,

				}
			});
			x += face->glyph->bitmap.width;
			if (face->glyph->bitmap.rows > current_max_height) {
				current_max_height = face->glyph->bitmap.rows;
			}
			if (face->glyph->bitmap.pitch > 0) { 
				// bitmap buffer stores pixels in decreasing vertical position. First bytes in the buffer correspond to
				// the topmost pixels in the bitmap.
				for (int i = 0; i < (*charmap)[current_char].size.y; i++) {
					std::cout << "3\n";

					glm::ivec2 tex_pos = glm::ivec2(x, y + ((*charmap)[current_char].size.y - i));
					std::cout << x << " " << (*charmap)[current_char].size.y << "\n";
					std::cout << tex_pos.y * tex_size + tex_pos.x << "\n";
					auto f = &((*bitmap_img)[tex_pos.y * tex_size + tex_pos.x]);
					std::cout << "help" << f << "help\n";
					

					memcpy(
						f,
						&(face->glyph->bitmap.buffer[i * face->glyph->bitmap.pitch]),
						(*charmap)[current_char].size.x
					);
					std::cout << "5\n";
				}
			}
			else {
				// bitmap buffer stores pixels in increasing vertical position. First bytes in the buffer correspond to
				// the bottom pixels in the bitmap.
				for (int i = 0; i < (*charmap)[current_char].size.y; i++) {

					glm::ivec2 tex_pos = glm::ivec2(x, y + i);
					memcpy(
						&((*bitmap_img)[tex_pos.y * tex_size + tex_size]),
						&(face->glyph->bitmap.buffer[i * face->glyph->bitmap.pitch]),
						(*charmap)[current_char].size.x
					);

				}
			}
			
		}
		y += current_max_height;
		if (x > current_max_width)
			current_max_width = x;
		x = 0;
	}
	(*bitmap_img_size) = glm::ivec2(tex_size, tex_size);

}




}