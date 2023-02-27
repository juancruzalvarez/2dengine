#include "renderer.hh"

#include "glad/glad.h"
#include <iostream>

#include <gtc/matrix_transform.hpp>

namespace graphics {

        
const float Renderer::kQuadVertices [] = { 
     0.5f,  0.5f, 
     0.5f, -0.5f,
    -0.5f, -0.5f,
    -0.5f,  0.5f, 
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
    glBufferData(GL_ARRAY_BUFFER, kMaxQuads * 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);

}

void Renderer::Render(Quad quad, uint32_t texture_id) {
    float t[12] = { quad.pos.x, quad.pos.y, quad.size.x, quad.size.y, quad.uvs[0], quad.uvs[1],quad.uvs[2], quad.uvs[3],
    quad.uvs[4], quad.uvs[5],quad.uvs[6], quad.uvs[7] };
    if (texture_id != current_texture_id_ || current_quad_ >= kMaxQuads) {
        Flush();
        current_texture_id_ = texture_id;
    }
    glBindBuffer(GL_ARRAY_BUFFER, instanced_data_vbo_id_);
    glBufferSubData(GL_ARRAY_BUFFER, current_quad_ * sizeof(t), sizeof(t), t);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    current_quad_++;
}

void Renderer::Flush() {
    glBindVertexArray(vao_id_);
    shader_.use();
    glActiveTexture(GL_TEXTURE0);                   
    glBindTexture(GL_TEXTURE_2D, current_texture_id_);
    shader_.set_int("tex", 0);
    float* d = new float[24];
    glBindBuffer(GL_ARRAY_BUFFER, instanced_data_vbo_id_);

    glGetBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float) * 2, (void*)d);
    std::cout << "DATA:" << "\n";
    for (int i = 0; i < 24; i++)
        std::cout << d[i] << "\n";
    std::cout << "---------------------"<<current_quad_<<"\n";
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, current_quad_);
    
    current_quad_ = 0;
}

void Renderer::SetOrtho(glm::ivec2 size) {
    auto mat = glm::ortho(0, size.x, 0, size.y);
    shader_.set_m4x4f("ortho", mat);
}


}