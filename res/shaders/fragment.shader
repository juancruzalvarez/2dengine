#version 430 core

out vec4 frag_color;


in vec2 uv_coords;
in vec4 p_color;
uniform sampler2D tex;

void main() {
	frag_color = p_color*vec4(texture(tex, uv_coords).r, texture(tex, uv_coords).r, texture(tex, uv_coords).r, texture(tex, uv_coords).r);
}