#include "app.hh"
#include "texture.hh"
#include "font_loading.hh"
#include "util.hh"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}


App::App() { 

	glfwInit();

	window_ = new Window{ "hello", 800, 800 };
	window_->make_context_current();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	
	window_->set_key_callback( [&](int glfw_key_code, int scancode, Input::Action action, int modifiers) {
		key_callback(glfw_key_code, scancode, action, modifiers);
	});
	
	renderer_ = new graphics::Renderer();

	uint8_t* bitmap_img;
	glm::ivec2 bitmap_img_size;
	graphics::LoadFont("res/fonts/CascadiaCode.ttf", 48, &bitmap_img, &bitmap_img_size, &charmap_);
	auto d = charmap_['a'];
	std::cout << "DATA:" << d.size.x << " " << d.size.y << "\n";
	std::cout << "DATA:" << d.bot_left_corner.x << " " << d.bot_left_corner.y << "\n" << d.top_right_corner.x << " " << d.top_right_corner.y << "\n";
	graphics::TextureCreateInfo info{
		GL_RGBA8,
		GL_NEAREST,
		GL_REPEAT,
		bitmap_img_size,
	};

	font_bitmap_ = graphics::Texture(
		info,
		GL_RED,
		GL_UNSIGNED_BYTE,
		(void*)bitmap_img
	);

	delete[] bitmap_img;
}
void App::Run() {
	while (!window_->should_close())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		renderer_->Render(
			graphics::Quad {
				{0.0,0.0},
				{2,2},
				util::pixel_corners_to_uv_array(
					charmap_['a'].bot_left_corner,
					charmap_['a'].top_right_corner,
					font_bitmap_.size)
			},
			font_bitmap_.texture_id);
		//renderer_->Render(graphics::Quad{ {0.3,0.5},{0.4,0.6},vec }, t.texture_id);

		renderer_->Flush();
		if (window_->has_resized())
			std::cout << "\nRESIZED!!!!" << "\n";
		window_->update();
	}
}

void App::key_callback(int glfw_key_code, int scancode, Input::Action action, int modifiers){
}

App::~App() {
	glfwTerminate();
}