#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <functional>

#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "input.hh"

class Window {

public:
	enum class ScreenMode {
		kFullscreen,
		kMaximized,
		kNormal,
	};


	Window(std::string title, int width, int height, bool fullscreen = false);
	void set_key_callback(Input::KeyCallbackFn callback);
	void set_char_callback(Input::CharCallbackFn callback);
	void set_screen_mode(ScreenMode screen_mode);
	void set_visible(bool visible);
	glm::vec2 framebuffer_size();
	void make_context_current();
	glm::vec2 window_size();
	bool should_close();
	bool has_resized();
	void update();

private:
	GLFWwindow *window_handle_;
	ScreenMode screen_mode_;
	bool has_resized_;
	glm::vec2 framebuffer_size_;
	glm::vec2 window_size_;
	Input::KeyCallbackFn key_callback_;
	Input::CharCallbackFn char_callback_;

};

#endif