#ifndef RENDERER_H_
#define RENDERER_H_

#include <array>

#include <glm.hpp>

#include "shader.hh"

namespace graphics {

	struct Quad {
		glm::vec2 pos;
		glm::vec2 size;
		std::array<float, 8> uvs;
	};

	class Renderer {
	public:
		Renderer();
		~Renderer();
		void Render(Quad quad, uint32_t texture_id);
		void Flush();
	private:
		static const float kQuadVertices[6*3];
		static const int kQuadIndices[6];
		static const int kMaxQuads;

		Shader shader_;
		uint32_t vao_id_;
		uint32_t instanced_data_vbo_id_;
		uint32_t current_texture_id_;
		int current_quad_;
	};
}

#endif
