#include "app.hh"
#include "texture.hh"
#include "font_loading.hh"

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
	uint8_t* bitmap_img;
	std::map<uint8_t, graphics::CharInfo> charmap;
	glm::ivec2 bitmap_img_size;
	graphics::LoadFont("res/fonts/CascadiaCode.ttf", 32, &bitmap_img, &bitmap_img_size , &charmap);
	std::cout << "charmap_size:" << charmap.size() << "\n";
	std::cout << "A:" << charmap['a'].size.x << " " << charmap['a'].size.y << "\n";
	glfwInit();
	window_ = new Window{"hello", 800, 800};
	window_->set_key_callback( [&](int glfw_key_code, int scancode, Input::Action action, int modifiers) {
		key_callback(glfw_key_code, scancode, action, modifiers);
	});
	window_->make_context_current();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	renderer_ = new graphics::Renderer();
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); 
	uint8_t* data = stbi_load("res/sprites/test.png", &width, &height, &nrChannels, 4);
	if (!data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	
	graphics::TextureCreateInfo info{
		GL_RGBA8,
		GL_LINEAR,
		GL_REPEAT,
		bitmap_img_size.x, bitmap_img_size.y
	};

	 t = graphics::Texture(
		info,
		GL_RED,
		GL_UNSIGNED_BYTE,
		(void*)bitmap_img
	);
}
void App::Run() {
	while (!window_->should_close())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		std::vector<float> vec = { 1, 1, 1, 0, 0,0, 0, 1 };
		renderer_->Render(graphics::Quad{ {0.0,0.0},{2,2},vec }, t.texture_id);
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