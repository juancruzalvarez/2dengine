#ifndef RENDERER_H_
#define RENDERER_H_

#include <glm.hpp>

#include "shader.hh"

namespace graphics
{
	typedef glm::vec4 RGBAColor;

	enum RENDERER_BRUSH_SHAPE
	{
		RENDERER_SQUARE,
		RENDERER_CIRCLE,
		RENDERER_LINE,
		RENDERER_BRUSH_SHAPE_COUNT
	};

	enum RENDERER_TEXTURES
	{
		RENDERER_DRAW_TEXTURE,
		RENDERER_SELECTED_TEXTURE,
		RENDERER_SURFACE_TEXTURE,
		RENDERER_TEXTURE_COUNT
	};

	enum RENDERER_SHADERS
	{
		RENDERER_BRUSH_SHADER,
		RENDERER_SHADER_COUNT
	};



	class Renderer 
	{
	public:
		Renderer();
		void set_surface(Surface* surface);
		void set_fg_color(RGBAColor color);
		void set_bg_color(RGBAColor color);
		void set_brush_shape(RENDERER_BRUSH_SHAPE shape);
		void set_brush_size(int size);
		void paint_dot(glm::vec2 pos);
		void update_img();
	private:
		uint32_t textures[RENDERER_TEXTURE_COUNT];
		uint32_t brush_vaos[RENDERER_BRUSH_SHAPE_COUNT];
		Shader shaders[RENDERER_SHADER_COUNT];
		Surface* current_surface;
		RGBAColor foreground_color;
		RGBAColor background_color;
		RENDERER_BRUSH_SHAPE brush_shape;
		int brush_size;
		uint8_t* circle_texture(int res);
	};
}


#endif
