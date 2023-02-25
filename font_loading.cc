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
	std::map<uint8_t, CharInfo>* char_map) {

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
	std::map<uint8_t, CharInfo> charmap;

	int x = 0, y = 0;
	int current_max_height = 0;
	int current_max_width = 0;
	for (int r = 0; r < 16; r++) {
		for (int c = 0; c < 16; c++) {
			uint8_t current_char = r * 16 + c;
			error = FT_Load_Char(face, current_char, FT_LOAD_RENDER);
			if (error) continue;
			charmap.insert({
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
			if (y + face->glyph->bitmap.rows > current_max_height) {
				current_max_height = y + face->glyph->bitmap.rows;
			}
			
		}
		y += current_max_height;
		if (x > current_max_width)
			current_max_width = x;
		x = 0;
	}

	std::cout << "After:" << current_max_width << " " << current_max_height << "\n";

}




}