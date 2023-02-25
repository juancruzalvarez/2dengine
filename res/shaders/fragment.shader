#version 430 core

out vec4 frag_color;

in vec2 uv_coords;
uniform sampler2D tex;

void main() {
	frag_color = texture(tex, uv_coords);
	//frag_color = vec4(uv_coords.x, uv_coords.y, 0, 1);
}