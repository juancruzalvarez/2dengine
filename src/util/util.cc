#include "util.hh"

#include <vector>
#include <iostream>

namespace util {

std::array<float, 8> pixel_corners_to_uv_array(glm::ivec2 bot_left_corner, glm::ivec2 top_right_corner, glm::ivec2 texture_size) {
	std::array<float, 8> uvs;
	auto bot_rigth_corner = glm::ivec2(top_right_corner.x, bot_left_corner.y);
	auto top_left_corner  = glm::ivec2(bot_left_corner.x, top_right_corner.y);
	glm::vec2 arr[4] = {top_right_corner, bot_rigth_corner, bot_left_corner, top_left_corner};
	for (auto &c : arr) {

		c.x /= float(texture_size.x);
		c.y /= float(texture_size.y);
	}
	memcpy(uvs.data(), arr, sizeof(arr));
	return uvs;
}

}