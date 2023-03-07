#ifndef FONT_LOADING_H
#define FONT_LOADING_H

#include <string>
#include <vector>
#include <map>
#include <glm.hpp>

namespace graphics {
/*
struct CharInfo {
	glm::ivec2 bot_left_corner, top_right_corner;
	glm::vec2 size;
	glm::vec2 bearing;
	float advance;
};
*/
void LoadFont(
	std::string path,
	int pixel_height,
	uint8_t** bitmap_img,
	glm::ivec2* bitmap_img_size,
	std::map<uint8_t, CharInfo>* char_map
);



}

#endif
