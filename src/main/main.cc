#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include "app.hh"

#include "file_utils.hh"

int main()
{
	App *app = new App();
	app->Run();
}