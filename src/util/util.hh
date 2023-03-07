#ifndef UTIL_H
#define UTIL_H

#include <array>

#include <glm.hpp>

namespace util {

	/**
	* Transform pixel coordinates to uvs.
	*
	* Given the corners in pixel coordinates of the section of texture required, and the size of the texture in pixels,
	* Returns an array with the uv coordinates.
	*
	* @param bot_left_corner  Coordinates of the left-bottom corner.
	* @param top_right_corner Coordinates of the right-top corner.
	*
	* @return Array containing the uv coordinates required to render the secction of the texture.
	*/
	std::array<float, 8> pixel_corners_to_uv_array(glm::ivec2 bot_left_corner, glm::ivec2 top_right_corner, glm::ivec2 texture_size);

}


#endif