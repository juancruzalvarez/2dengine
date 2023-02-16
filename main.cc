#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include "window.hh"

const int WIN_WIDTH = 1000, WIN_HEIGHT = 800;
const char* WIN_TITLE = "PIXEL!";

int main()
{
	glfwInit();
	Window window{"Test", 500, 500};

	while (!window.should_close())
	{
		if (window.has_resized())
			std::cout << "\nRESIZED!!!!" << "\n";
		window.update();
	}
	std::cout << "Window Size:" << window.window_size().x << " " << window.window_size().y << "\n";
	std::cout << "Framebuffer Size:" << window.framebuffer_size().x << " " << window.framebuffer_size().y << "\n";

	glfwTerminate();
}