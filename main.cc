#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

GLFWwindow* window;
const int WIN_WIDTH = 1000, WIN_HEIGHT = 800;
const char* WIN_TITLE = "PIXEL!";

int main()
{
	glfwInit();
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}