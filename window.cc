#include "window.hh"

#include <functional>
#include <iostream>

#include <GLFW/glfw3.h>
Window::Window(std::string title, int width, int height, bool fullscreen) {
	m_framebuffer_size = { width, height };
	m_window_size = { width, height };
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window_handle = glfwCreateWindow(
		width,
		height,
		title.c_str(),
		fullscreen ? glfwGetPrimaryMonitor() : nullptr,
		nullptr
	);
	glfwSetWindowUserPointer(m_window_handle, this);
	glfwSetFramebufferSizeCallback(
		m_window_handle,
		[](GLFWwindow *window, int width, int height){
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->m_framebuffer_size = glm::vec2(width, height);
			usr_ptr->m_has_resized = true;
			std::cout << "resize";
		}
	);

	glfwSetWindowSizeCallback(
		m_window_handle,
		[](GLFWwindow* window, int width, int height) {
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->m_window_size = glm::vec2(width, height);
			usr_ptr->m_has_resized = true;
			std::cout << "resize";
		}
	);
	glfwSetWindowMaximizeCallback(
		m_window_handle,
		[](GLFWwindow* window, int maximized) {
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->m_screen_mode = maximized ? ScreenMode::kMaximized : ScreenMode::kNormal;
			std::cout << "maximized";
		}
	);

}

void Window::SetKeyCallback(input::KeyCallbackFn callback) {
	m_key_callback = callback;
	glfwSetKeyCallback(
		m_window_handle,
		[](GLFWwindow* window, int key, int scancode, int action, int modifiers) {
			input::Action a;
			switch (action) {
			case GLFW_PRESS: a = input::Action::kPressed; break;
			case GLFW_RELEASE: a = input::Action::kReleased; break;
			case GLFW_REPEAT: a = input::Action::kRepeat; break;
			default: return;
			}
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->m_key_callback(key, scancode, a, modifiers);
		}
	);
}

void Window::SetCharCallback(input::CharCallbackFn callback) {
	m_char_callback = callback;
	glfwSetCharCallback(
		m_window_handle,
		[](GLFWwindow* window, uint32_t character) {
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->m_char_callback(character);
		}
	);
}



void Window::MakeContextCurrent() {
	glfwMakeContextCurrent(m_window_handle);
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(m_window_handle);
}

bool Window::HasResized() {
	return m_has_resized;
}

void Window::Update() {
	m_has_resized = false;
	glfwPollEvents();
	glfwSwapBuffers(m_window_handle);
}

void Window::SetScreenMode(ScreenMode screen_mode) {
	switch (screen_mode)
	{
		case ScreenMode::kFullscreen: {
			glfwSetWindowMonitor(m_window_handle, glfwGetPrimaryMonitor(), 0, 0, m_window_size.x, m_window_size.y, GLFW_DONT_CARE);
		}
		case ScreenMode::kMaximized: {
			glfwSetWindowMonitor(m_window_handle, nullptr, 0, 0, m_window_size.x, m_window_size.y, GLFW_DONT_CARE);
			glfwMaximizeWindow(m_window_handle);
		}
		case ScreenMode::kNormal: {
			glfwSetWindowMonitor(m_window_handle, nullptr, 0, 0, m_window_size.x, m_window_size.y, GLFW_DONT_CARE);
		}
	}
}

void Window::SetVisible(bool visible) {
	visible ? glfwShowWindow(m_window_handle) : glfwHideWindow(m_window_handle);
}

glm::vec2 Window::FramebufferSize() {
	return m_framebuffer_size;
}

glm::vec2 Window::WindowSize() {
	return m_window_size;
}
