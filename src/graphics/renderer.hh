#ifndef RENDERER_H_
#define RENDERER_H_

#include <array>

#include <glm.hpp>

#include "shader.hh"
#include "texture.hh"
#include <map>
namespace graphics {

	struct CharInfo {
		glm::ivec2 bot_left_corner, top_right_corner;
		glm::dvec2 size;
		glm::dvec2 bearing;
		double advance;
	};

	struct Quad {
		glm::vec2 pos;
		glm::vec2 size;
		std::array<float, 8> uvs;
		glm::vec4 color;
	};

	class Renderer {
	public:
		Renderer();
		~Renderer();
		void Render(Quad quad, uint32_t texture_id);
		void RenderWhiteQuad(Quad quad);
		void Flush();
		void SetOrtho(glm::vec2 size);
		void RenderText(std::string text, glm::vec2 pos, int pixel_height, glm::vec4 color);
		glm::vec2 GetTextSize(std::string text, int pixel_height);
		void LoadFont(std::string path, int pixel_height);
	private:
		static const float kQuadVertices[6*3];
		static const int kQuadIndices[6];
		static const int kMaxQuads;

		Shader shader_;
		Texture m_white_square_texture;
		Texture font_texture_;
		std::map<uint8_t, CharInfo> charmap_;
		double m_font_info_max_under_baseline;
		uint32_t vao_id_;
		uint32_t instanced_data_vbo_id_;
		uint32_t current_texture_id_;
		int current_quad_;
	};
}

#endif
