#version 430 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec4 transform;
layout (location = 2) in vec4 uvs1;
layout (location = 3) in vec4 uvs2;

out vec2 uv_coords; 

void main()
{
    gl_Position = vec4(in_pos * transform.zw + transform.xy, 0.0, 1.0);
    switch (gl_VertexID) {
    case 0: uv_coords = uvs1.xy; break;
    case 1: uv_coords = uvs1.zw; break;
    case 2: uv_coords = uvs2.xy; break;
    case 3: uv_coords = uvs2.zw; break;
    }
}