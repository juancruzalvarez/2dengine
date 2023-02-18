#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include "window.hh"

const int WIN_WIDTH = 1000, WIN_HEIGHT = 800;
const char* WIN_TITLE = "PIXEL!";

void callback(int key, int b, int c, int d) {
	if (c == GLFW_PRESS)std::cout << "Key " << key << "was pressed.\n";
	if (c == GLFW_RELEASE)std::cout << "Key " << key << "was released.\n";
}
int main()
{
	glfwInit();
	Window window{"Test", 500, 500};
	window.set_key_callback(callback);
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