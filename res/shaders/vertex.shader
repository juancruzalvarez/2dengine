#version 430 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec4 transform;
layout (location = 2) in vec4 uvs1;
layout (location = 3) in vec4 uvs2;
layout (location = 4) in vec4 color;


uniform vec2 ortho_size;

out vec2 uv_coords; 
out vec4 p_color;

void main()
{
    vec2 pos = ((in_pos * transform.zw + transform.xy) / ortho_size)*2 - 1;
    gl_Position = vec4(pos, 0.0, 1.0);
    vec2 uvs[4] = { uvs1.xy, uvs1.zw, uvs2.xy, uvs2.zw };
    uv_coords = uvs[gl_VertexID];
    p_color = color;
}