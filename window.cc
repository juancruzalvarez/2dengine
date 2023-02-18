#include "window.hh"

#include <functional>
#include <iostream>

#include <GLFW/glfw3.h>
Window::Window(std::string title, int width, int height, bool fullscreen) {
	window_handle_ = glfwCreateWindow(
		width,
		height,
		title.c_str(),
		fullscreen ? glfwGetPrimaryMonitor() : nullptr,
		nullptr
	);
	glfwSetWindowUserPointer(window_handle_, this);

	glfwSetFramebufferSizeCallback(
		window_handle_,
		[](GLFWwindow *window, int width, int height){
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->framebuffer_size_ = glm::vec2(width, height);
			usr_ptr->has_resized_ = true;
			std::cout << "resize";
		}
	);

	glfwSetWindowSizeCallback(
		window_handle_,
		[](GLFWwindow* window, int width, int height) {
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->window_size_ = glm::vec2(width, height);
			usr_ptr->has_resized_ = true;
			std::cout << "resize";
		}
	);
	glfwSetWindowMaximizeCallback(
		window_handle_,
		[](GLFWwindow* window, int maximized) {
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->screen_mode_ = maximized ? ScreenMode::kMaximized : ScreenMode::kNormal;
			std::cout << "maximized";
		}
	);
}

void Window::set_key_callback(std::function<void(int key, int scancode, Action action, int modifiers)>callback) {
	key_callback_ = callback;
	glfwSetKeyCallback(
		window_handle_,
		[](GLFWwindow* window, int key, int scancode, int action, int modifiers) {
			Action a;
			switch (action) {
			case GLFW_PRESS: a = Action::kPressed; break;
			case GLFW_RELEASE: a = Action::kReleased; break;
			case GLFW_REPEAT: a = Action::kRepeat; break;
			}
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->key_callback_(key, scancode, a, modifiers);
		}
	);
}

void Window::set_char_callback(std::function<void(int utf32_character)> callback) {
	char_callback_ = callback;
	glfwSetCharCallback(
		window_handle_,
		[](GLFWwindow* window, int character) {
			Window* usr_ptr = ((Window*)glfwGetWindowUserPointer(window));
			usr_ptr->char_callback_(character);
		}
	);
}



void Window::make_context_current() {
	glfwMakeContextCurrent(window_handle_);
}

bool Window::should_close() {
	return glfwWindowShouldClose(window_handle_);
}

bool Window::has_resized() {
	return has_resized_;
}

void Window::update() {
	has_resized_ = false;
	glfwPollEvents();
	glfwSwapBuffers(window_handle_);
}

void Window::set_screen_mode(ScreenMode screen_mode) {
	switch (screen_mode)
	{
		case ScreenMode::kFullscreen: {
			glfwSetWindowMonitor(window_handle_, glfwGetPrimaryMonitor(), 0, 0, window_size_.x, window_size_.y, GLFW_DONT_CARE);
		}
		case ScreenMode::kMaximized: {
			glfwSetWindowMonitor(window_handle_, nullptr, 0, 0, window_size_.x, window_size_.y, GLFW_DONT_CARE);
			glfwMaximizeWindow(window_handle_);
		}
		case ScreenMode::kNormal: {
			glfwSetWindowMonitor(window_handle_, nullptr, 0, 0, window_size_.x, window_size_.y, GLFW_DONT_CARE);
		}
	}
}

void Window::set_visible(bool visible) {
	visible ? glfwShowWindow(window_handle_) : glfwHideWindow(window_handle_);
}

glm::vec2 Window::framebuffer_size() {
	return framebuffer_size_;
}

glm::vec2 Window::window_size() {
	return window_size_;
}
